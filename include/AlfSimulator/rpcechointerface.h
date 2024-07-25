#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include "DimRpcParallel/dimrpcparallel.h"
#include "AlfSimulator/print.h"
#include "AlfSimulator/utility.h"
#include "AlfSimulator/main.h"
#include "IPbusInterface.h"


using namespace std;

const int SWT_HIGH_WIDTH = 76 / 4; // SWT low hexadecimal width
const int SCA_HIGH_WIDTH = 32 / 4; // SCA low hexadecimal width

class RpcEcho : public DimRpcParallel
{

    void rpcHandler()
    {
        string request, response;

        request = getString();
        PrintVerbose(string("RPC handler\n"));
        PrintVerbose(string("RPC request received at " + rpcName + ":\n") + request);

        std::map<std::string, int32_t> myMap = {
            {"ALF_42/SERIAL_4/LINK_4/SWT_SEQUENCE", 0x0005}
        };
        std::cout<<std::to_string(myMap[rpcName]);


        /*boost::asio::io_context io_service;
        IPbusTarget target(io_service,"172.20.75.180", 0, 50001);
        target.checkStatus();
        //target.debug_mode(IPbusTarget::DebugMode::Vital);
        target.timer_tick(std::chrono::seconds(2));
        target.start_timer();
        IPbusControlPacket packet;

        uint32_t data[SIZE] = {0x0,0x0};
        packet.addTransaction(TransactionType::ipread, myMap[rpcName], data, SIZE);
        target.transcieve(packet);

        std::cout << "\n\n\tRead...\n\n";
        std::cout << std::hex << data[0] << ' ' << std::hex << data[1] << std::endl;  
*/



        response = handleRequest(request);

        if (config->getResponseDelay())
            this_thread::sleep_for(chrono::milliseconds(config->getResponseDelay()));

        PrintVerbose(string("RPC echo response sent:\n") + response);

        setData((char *)response.c_str());
    }

    string handleRequest(string request);

protected:
    string rpcName;

public:
    RpcEcho(string name, int bankId) : DimRpcParallel((char *)name.c_str(), "C", "C", bankId)
    {
        rpcName = name;
    };
};