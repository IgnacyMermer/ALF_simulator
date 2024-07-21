#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include "DimRpcParallel/dimrpcparallel.h"
#include "AlfSimulator/print.h"
#include "AlfSimulator/utility.h"
#include "AlfSimulator/main.h"
#include "AlfSimulator/rpccustomhandler.h"

using namespace std;

const int SWT_HIGH_WIDTH = 76 / 4; // SWT low hexadecimal width
const int SCA_HIGH_WIDTH = 32 / 4; //SCA width is 32 bits for every protocol except SWT

class RpcEcho : public DimRpcParallel
{

    void rpcHandler();

    string handleRequest(string request);

    string rpcName;

    RpcCustomHandler* handler = nullptr;

private:
    
public:
    RpcEcho(string name, int bankId) : DimRpcParallel((char *)name.c_str(), "C", "C", bankId)
    {
        rpcName = name;
    };

    void setRpcCustomHandler(RpcCustomHandler* handler)
    {
        this->handler = handler;
    }
};