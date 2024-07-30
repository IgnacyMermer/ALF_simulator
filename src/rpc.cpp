#include "AlfSimulator/rpc.h"
#include <sstream>
#include <iomanip>
#include "TCM.h"
//#include "IPbusInterface.h"
#include "FITConnection.h"
#include <bitset>
#include <array>

#define SIZE 2

std::string toHexString(int32_t value) {
    std::stringstream ss;
    ss << std::hex << value;
    std::string hexString = ss.str();
    std::string finalHex = "";
    for(int i=0; i<19-hexString.length(); i++){
        finalHex+="0";
    }
    finalHex+=hexString;
    return finalHex;
}

void RpcEcho::rpcHandler()
{
    string request, response;

    request = getString();
    //PrintVerbose(string("RPC request received at " + rpcName + ":\n") + request);

    if (this->handler == nullptr){
        response = handleRequest(request);
    }
    else{
        response = this->handler->handleRequest(request);
    }

    if (config->getResponseDelay())
        this_thread::sleep_for(chrono::milliseconds(config->getResponseDelay()));

    //PrintVerbose(string("RPC echo response sent:\n") + response);
    setData((char *)response.c_str());
}

string RpcEcho::handleRequest(string request){
    string response = "success\n";
    if (rpcName.find("SCA_SEQUENCE") != string::npos)
    {
        vector<string> lines = Utility::splitString(request, "\n");

        for (size_t i = 0; i < lines.size(); i++)
        {
            string responseLine;
            if (Utility::isComment(lines[i])) 
                continue;

            if (lines[i].find("svl_connect") != string::npos)
                responseLine = "svl_connect";
            else if (lines[i].find("svl_reset") != string::npos || lines[i].find("sc_reset") != string::npos || lines[i].find("lock") != string::npos)
                continue;
            responseLine =lines[i].substr(0, lines[i].find(",")) + ",0x" + Utility::randomString(SCA_HIGH_WIDTH);
            response += responseLine + "\n";
        }
    }
    else if (rpcName.find("SWT_SEQUENCE") != string::npos)
    {
        vector<string> lines = Utility::splitString(request, "\n");

        int32_t commandAddress = -1;

        for (size_t i = 0; i < lines.size(); i++){
            std::string requestData = lines[i].substr(0, lines[i].find(","));
            string responseLine;
            if (Utility::isComment(lines[i])) 
                continue;
            else if (lines[i] == "reset")
                responseLine = "success";
            else if (lines[i].find(",write") != string::npos){
                
                TCMAddresses tcmAddresses;
                std::map<std::string, std::array<int32_t, 2>>& tcmMap = tcmAddresses.TCMAddressesMap;
                std::string address = requestData.substr(5, 6);
                commandAddress = tcmMap[address][0];
                
                if(requestData[requestData.length()-1]=='1'){
                    
                    IPbusControlPacket packet;
                    std::string address = requestData.substr(5, 6);

                    std::cout<<"save to electronics\n";
                    std::cout<<tcmMap[address][0];
                    std::cout<<"\n";

                    uint32_t data[SIZE] = {0x0,0x0};

                    unsigned long hex_number;
                    std::stringstream ss;
                    ss << std::hex << ("0x"+requestData.substr(11, requestData.length()-13));
                    ss >> hex_number;

                    data[0] = hex_number;

                    std::cout<<std::to_string(hex_number);
                    std::cout<<"\n";

                    if(tcmMap[address][1]==1){
                        packet.addTransaction(TransactionType::ipwrite, commandAddress, data, SIZE);
                        IPbusTarget& target = FITConnection::getTarget();
                        target.transcieve(packet);
                    }                
                }
                responseLine = "0";
            }
            else if (lines[i].find("read") != string::npos){
                TCMAddresses tcmAddresses;
                std::map<std::string, std::array<int32_t, 2>>& tcmMap = tcmAddresses.TCMAddressesMap;
                
                IPbusControlPacket packet;

                if(commandAddress==-1){
                    responseLine="0x1";
                }
                else{
                    uint32_t data[SIZE] = {0x0,0x0};
                    packet.addTransaction(TransactionType::ipread, commandAddress, data, SIZE);
                    IPbusTarget& target = FITConnection::getTarget();
                    target.transcieve(packet);
                    responseLine = "0x" + toHexString(data[0]);
                }
            }
            else if (lines[i].find("wait") != string::npos){
                if (lines[i].find(",") == string::npos)
                    responseLine = "3"; 
                else
                    responseLine = lines[i].substr(0,lines[i].find(","));
            }
            if (!(lines[i].find("sc_reset") != string::npos || lines[i].find("lock") != string::npos || lines[i].find("reset") != string::npos))
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
            
            if (lines[i].find("write") != string::npos){
                vector<string> data = Utility::splitString(lines[i],",");
                std::ostringstream responseStream;
                size_t prefixPos = data[1].find("0x");
                if(prefixPos == string::npos) 
                    prefixPos = data[1].find("0X");
                responseStream << "0x" << std::setw(SCA_HIGH_WIDTH) << std::setfill('0') << data[1].substr(prefixPos+2);
                responseLine = responseStream.str();
            }
            else if (lines[i].find("read") != string::npos){
                responseLine = "0x" + Utility::randomString(SCA_HIGH_WIDTH);
            }

            if (lines[i].find("lock") == string::npos)
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
        response = "success\n\n";
    }

    if(response[response.size()-1] == '\n')
        response.erase(response.size() - 1); // Remove last character ("\n") from response if present

    return response;
}

