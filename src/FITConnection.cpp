#include "FITConnection.h"
//#include "IPbusInterface.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "IPbusControlPacket.h"
#include <chrono>
#include <iostream>


std::unique_ptr<IPbusTarget> FITConnection::target = nullptr;

void FITConnection::initialize(const std::string& ip, int port, int timeout) {
    boost::asio::io_context io_service;
    target = std::unique_ptr<IPbusTarget>(new IPbusTarget(io_service,"172.20.75.180", 0, 50001));
    target->checkStatus();
    target->timer_tick(std::chrono::seconds(2));
    target->start_timer();
    IPbusControlPacket packet;
}

IPbusTarget& FITConnection::getTarget() {
    return *target;
}
