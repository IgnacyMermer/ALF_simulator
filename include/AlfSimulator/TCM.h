#ifndef TCM_H
#define TCM_H

#include <unordered_map>
#include "./Parameter.h"
#include <cstdint>
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
    std::map<std::string, int32_t> TCMAddressesMap = {
        {"ALF_42/SERIAL_4/LINK_4/SWT_SEQUENCE", 0x0005}
    };
};


#endif // TCM_H
