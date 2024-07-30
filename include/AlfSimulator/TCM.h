#ifndef TCM_H
#define TCM_H

#include <unordered_map>
#include "./Parameter.h"
#include <cstdint>
#include <array>
//#include "FITboardsCommon.h"

const double LHCclock_MHz = 40.0789658; //reference value
const int countersUpdatePeriod_ms[8] = {0, 100, 200, 500, 1000, 2000, 5000, 10000};
extern double systemClock_MHz; //40
extern double TDCunit_ps; // 13
extern double halfBC_ns; // 25
extern double phaseStepLaser_ns, phaseStep_ns;

struct regblock {
    const uint8_t addr, endAddr;
    inline uint8_t size() { return endAddr - addr + 1; }
};

struct TypeTCM {
    struct ActualValues {
        
        uint32_t *registers = (uint32_t *)this;
        
    } act;

};

struct TCMAddresses {
    // {address from FRED in SWT, {address in FIT electronics for IPbus, write can./ be proceed}}
    std::map<std::string, std::array<int32_t, 2>> TCMAddressesMap = {
        {"111222", {0x006A, 1}}, //triggers
        {"111333", {0x00E8, 0}}, //status bits
        {"111000", {0x000F, 0}}, //board status
        {"100000", {-1,     0}}, //return 1 if alf server is running
        {"111444", {0x0000, 0}}, //delay A
        {"111555", {0x0001, 0}}, //delay C
        {"111666", {0x0002, 0}}, //delay laser
        {"111777", {0x0005, 0}} //board temperature
    };
};


#endif // TCM_H
