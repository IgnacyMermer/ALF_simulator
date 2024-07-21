#include "AlfSimulator/rpc.h"
#include <sstream>
#include <iomanip>
#include "TCM.h"
//#include "IPbusInterface.h"
#include "FITConnection.h"

#define SIZE 2

std::string toHexString(int32_t value) {
    std::stringstream ss;
    ss << std::hex << value;
    return ss.str();
}

void RpcEcho::rpcHandler()
{
    string request, response;

    request = getString();
    PrintVerbose(string("RPC request received at " + rpcName + ":\n") + request);

    if (this->handler == nullptr)
    {
        response = handleRequest(request);
    }
    else
    {
        response = this->handler->handleRequest(request);
    }

    if (config->getResponseDelay())
        this_thread::sleep_for(chrono::milliseconds(config->getResponseDelay()));

    PrintVerbose(string("RPC echo response sent:\n") + response);
    setData((char *)response.c_str());
}

string RpcEcho::handleRequest(string request)
{
    std::cout<<rpcName<<'\n';
    string response = "success\n"; // Response strings are prefixed by success\n or failure\n in RPC, for now success\n only
    if (rpcName.find("SCA_SEQUENCE") != string::npos)
    {
        vector<string> lines = Utility::splitString(request, "\n");

        for (size_t i = 0; i < lines.size(); i++)
        {
            string responseLine;
            if (Utility::isComment(lines[i])) 
                continue;

            if (lines[i].find("svl_connect") != string::npos) // Return svl_connect as confirmation
                responseLine = "svl_connect";
            else if (lines[i].find("svl_reset") != string::npos || lines[i].find("sc_reset") != string::npos || lines[i].find("lock") != string::npos) // svl_reset,sc_reset and lock return nothing so skip response build
                continue;
            responseLine = // For SCA protocol, Response line is equal to "sca_command,sca_read_data", copy command and generate random data
                lines[i].substr(0, lines[i].find(",")) + ",0x" + Utility::randomString(SCA_HIGH_WIDTH);
            response += responseLine + "\n";
        }
        // response = response;
    }
    else if (rpcName.find("SWT_SEQUENCE") != string::npos)
    {
        vector<string> lines = Utility::splitString(request, "\n");


        TCMAddresses tcmAddresses;
        std::map<std::string, int32_t>& tcmMap = tcmAddresses.TCMAddressesMap;
        std::cout<<std::to_string(tcmMap[rpcName]);

        
        IPbusControlPacket packet;

        uint32_t data[SIZE] = {0x0,0x0};
        packet.addTransaction(TransactionType::ipread, tcmMap[rpcName], data, SIZE);
        IPbusTarget& target = FITConnection::getTarget();
        //FITConnection::target->transcieve(packet);
        target.transcieve(packet);

        std::cout << std::hex << data[0] << ' ' << std::hex << data[1] << std::endl;




        for (size_t i = 0; i < lines.size(); i++)
        {
            string responseLine;
            std::cout<<(lines[i]);
            if (Utility::isComment(lines[i])) 
                continue;
            else if (lines[i] == "reset")
                responseLine = "success";
            else if (lines[i].find(",write") != string::npos)
                responseLine = "0";
            else if (lines[i].find("read") != string::npos)
                responseLine = "0x" + /*Utility::randomString(SWT_HIGH_WIDTH)*/toHexString(data[0]);
            else if (lines[i].find("wait") != string::npos)
            {
                if (lines[i].find(",") == string::npos)
                    responseLine = "3"; // Default wait time return, From O2Alf Readme
                else
                    responseLine = lines[i].substr(0,lines[i].find(",")); // time,wait -> substring time from line (return time on wait command)
            }
            if (!(lines[i].find("sc_reset") != string::npos || lines[i].find("lock") != string::npos)) // Commands sc_reset and lock should output nothing, add response only to other commands
                response += responseLine + "\n";
        }
    }
    else if (rpcName.find("IC_SEQUENCE") != string::npos)
    {
        vector<string> lines = Utility::splitString(request, "\n");
        for (size_t i = 0; i < lines.size(); i++)
        {
            if (Utility::isComment(lines[i])) 
                continue;

            string responseLine;
            
            if (lines[i].find("write") != string::npos) // IC_SEQUENCE Write command : address,value,write\n -> response echo value
            {
                vector<string> data = Utility::splitString(lines[i],","); //split command data by comma
                std::ostringstream responseStream;
                size_t prefixPos = data[1].find("0x"); //Find position of 0x, if not present find 0X (upper case X). Then remove 0x and append leading zeros 
                if(prefixPos == string::npos) 
                    prefixPos = data[1].find("0X");
                responseStream << "0x" << std::setw(SCA_HIGH_WIDTH) << std::setfill('0') << data[1].substr(prefixPos+2);// value is second parameter in write command (data[1]), fill value with leading zeros
                responseLine = responseStream.str();
            }
            else if (lines[i].find("read") != string::npos)
            {
                responseLine = "0x" + Utility::randomString(SCA_HIGH_WIDTH); // response is hex value read from register
            }

            if (lines[i].find("lock") == string::npos) // Add responseLine to response only if command is not "lock" (lock responds nothing)
                response += responseLine + "\n"; 

        }
    }
    else if (rpcName.find("REGISTER_SEQUENCE") != string::npos)
    {
        vector<string> lines = Utility::splitString(request,"\n");
        for (size_t i = 0; i < lines.size(); i++)
        {
            if (Utility::isComment(lines[i])) 
                continue;

            string responseLine;

            if (lines[i].find(",") != string::npos) // When comma is present, means write command -> response 0
                responseLine = "0";
            else if (lines[i].find("0x") != string::npos || lines[i].find("0X") != string::npos) // When only hex address is present this means read command -> response random hex number
                responseLine = "0x" + Utility::randomString(SCA_HIGH_WIDTH);
            else
                continue;
            response += responseLine + "\n";
        }
    }
    else // For IC_GBT_I2C_WRITE, PATTERN_PLAYER, LLA_SESSION_START, LLA_SESSION_STOP, RESET_CARD is response always empty string so skip.
    { 
        std::cout<<"chehehhe";
        response = "success\n\n";
    }

    if(response[response.size()-1] == '\n')
        response.erase(response.size() - 1); // Remove last character ("\n") from response if present

    return response;
}

