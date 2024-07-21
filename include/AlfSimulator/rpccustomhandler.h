#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include "AlfSimulator/print.h"
#include "AlfSimulator/utility.h"
#include "AlfSimulator/main.h"

using namespace std;

class RpcCustomHandler
{


  
public:
    virtual string handleRequest(string request) = 0;

};