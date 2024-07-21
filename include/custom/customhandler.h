#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include "AlfSimulator/print.h"
#include "AlfSimulator/utility.h"
#include "AlfSimulator/rpccustomhandler.h"

using namespace std;

class CustomHandler : public RpcCustomHandler
{

    string handleRequest(string request);

};