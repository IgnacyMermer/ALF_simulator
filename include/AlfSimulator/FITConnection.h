

#ifndef FITCONNECTION_H
#define FITCONNECTION_H

#include "IPbusInterface.h"
//#include <boost/asio.hpp>
//#include <boost/bind.hpp>
#include "IPbusControlPacket.h"
#include <memory>
#include <chrono>
#include <string>

class FITConnection {
public:
    static void initialize(const std::string& ip, int port, int timeout);
    static IPbusTarget& getTarget();

private:
    static std::unique_ptr<IPbusTarget> target;
};

#endif // FITCONNECTION_H
