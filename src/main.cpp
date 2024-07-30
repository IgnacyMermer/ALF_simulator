#include <memory>
#include "AlfSimulator/print.h"
#include "AlfSimulator/rpc.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include <cstdlib>
#include "AlfSimulator/main.h"
#include "custom/customhandler.h"
#include "IPbusControlPacket.h"
//#include "IPbusInterface.h"
//#include <QCoreApplication>
#include "./TCM.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "FITConnection.h"


AlfSimulatorConfig *config;

std::map<std::string, std::shared_ptr<RpcEcho>> RpcTopicMap;

void registerCustomRpc(string alf_name)
{
    CustomHandler *customHandler = new CustomHandler();
    string topic = alf_name + "/SERIAL_0/LINK_0/SCA_SEQUENCE";
    if (RpcTopicMap.find(topic) != RpcTopicMap.end()) //Check if topic is present in RpcTopicMap.
    {
        RpcTopicMap[alf_name + "/SERIAL_0/LINK_0/SCA_SEQUENCE"]->setRpcCustomHandler(customHandler); // Example custom RPC handler for SERIAL_0/LINK_0/SCA_SEQUENCE
    }
    else
    {
        PrintWarning("RPC ["+topic+"] Not available!");
    }
}

int main(int argc, char **argv)
{

    short STATUS=1;


    AlfSimulatorConfig cfg(argc, argv);
    config = &cfg;

    FITConnection::initialize("172.20.75.180", 0, 50001);

    IPbusTarget& target = FITConnection::getTarget();

    target.checkStatus();

    IPbusControlPacket packet;


    if (!std::getenv("DIM_DNS_NODE"))
    {
        PrintError("Environment variable \"DIM_DNS_NODE\" not set!");
        return 1;
    }

    // generate ALF name prefix / DNS name
    std::string alf_name = "ALF_";
    alf_name += cfg.getAlfId();
    PrintInfo("ALF Simulator with DNS name \"" + alf_name + "\" is running!");

    /// list of available topic endpoints
    std::vector<std::string> topic_endpoints{
        "SCA_SEQUENCE",
        "SWT_SEQUENCE",
        "IC_SEQUENCE",
        "IC_GBT_I2C_WRITE",
    };

    std::vector<std::string> serial_endpoints{
        "REGISTER_SEQUENCE",
        "PATTERN_PLAYER",
        "LLA_SESSION_START",
        "LLA_SESSION_STOP",
        "RESET_CARD" // CRORC Card RPC 
    };
    /// list of created topics
    vector<std::shared_ptr<RpcEcho>> topics;

    // generate serial interfaces
    for (int serial_id = 0; serial_id < cfg.getSerialCount(); serial_id++)
    {
        for (std::string &endpoint : serial_endpoints)
        {
            // Register SERIAL endpoints
            std::string topic_name(alf_name);
            topic_name += "/SERIAL_" + to_string(serial_id);
            topic_name += "/"+endpoint;

            std::shared_ptr<RpcEcho> rpcEcho = std::make_shared<RpcEcho>(topic_name, serial_id * 100 );

            topics.push_back(rpcEcho);
            RpcTopicMap[topic_name] = rpcEcho;

        }
        // generate lines per serial interface
        for (int link_id = 0; link_id < cfg.getLinkCount(); link_id++)
        {
            // generate all endpoints
            for (std::string &endpoint : topic_endpoints)
            {
                
                if (cfg.isEndpoint()) // If ALFSimulator in ENDPOINT_MODE, add ENDPOINT_[0,1] to topic_name
                {
                    for(size_t endpoint_id=0;endpoint_id<=1;endpoint_id++)
                    {
                        // build topic name
                        std::string topic_name(alf_name);
                        topic_name += "/SERIAL_" + to_string(serial_id);
                        topic_name += "/ENDPOINT_" + to_string(endpoint_id);
                        topic_name += "/LINK_" + to_string(link_id);
                        topic_name += "/" + endpoint;

                        // register topic
                        PrintVerbose("RPC \"" + topic_name + "\" registered");
                        // bank ID = serial_id * 100 + link_id
                        // see: https://github.com/AliceO2Group/ALF/blob/7d8a17b1f9f9cb465d75d5da5c3070f215f7357f/src/AlfServer.cxx#L548
                        std::shared_ptr<RpcEcho> rpcEcho = std::make_shared<RpcEcho>(topic_name, serial_id * 100 + link_id);

                        topics.push_back(rpcEcho);
                        RpcTopicMap[topic_name] = rpcEcho;
                    }
                }

                else // If Not in ENDPOINT_MODE, register only LINK to topic
                {
                    // build topic name
                    std::string topic_name(alf_name);
                    topic_name += "/SERIAL_" + to_string(serial_id);
                    topic_name += "/LINK_" + to_string(link_id);
                    topic_name += "/" + endpoint;

                    // register topic
                    PrintVerbose("RPC \"" + topic_name + "\" registered");
                    // bank ID = serial_id * 100 + link_id
                    // see: https://github.com/AliceO2Group/ALF/blob/7d8a17b1f9f9cb465d75d5da5c3070f215f7357f/src/AlfServer.cxx#L548
                    std::shared_ptr<RpcEcho> rpcEcho = std::make_shared<RpcEcho>(topic_name, serial_id * 100 + link_id);

                    topics.push_back(rpcEcho);
                    RpcTopicMap[topic_name] = rpcEcho;
                }
            }
            
        }
    }

    DimService statusService("ALF_42/WORK_STATUS",STATUS);

    registerCustomRpc(alf_name);
    // start DIM server
    DimServer::start(alf_name.c_str());

    // keep running
    PrintInfo("press CTRL-C to exit...");
    while (true){}


    // stop DIM server
    DimServer::stop();

    return 0;
}
