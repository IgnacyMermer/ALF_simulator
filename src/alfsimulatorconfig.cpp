#include "AlfSimulator/alfsimulatorconfig.h"
#include "AlfSimulator/print.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <iostream>


AlfSimulatorConfig::AlfSimulatorConfig(int argc, char **argv) {
    // check if user requests help
    if (hasArgument(argc, argv, "-h", "--help")) {
        print_help();
        std::exit(0);
    }

    // parse optional arguments
    this->serial_count = getUnsignedInt(argc, argv, "-s", "--serials", 3);
    this->link_count = getUnsignedInt(argc, argv, "-l", "--links", 5);
    this->response_delay = getUnsignedInt(argc, argv, "-d", "--delay", 0);


    // check probable bugs
    if (this->serial_count == 0)
        PrintWarning("Serial count is equal to 0");
    if (this->link_count == 0)
        PrintWarning("Link count is equal to 0");

    // check if verbose flag is specified
    this->verbose = hasArgument(argc, argv, "-v", "--verbose");

    // check if endpoint flag is specified
    this->endpoint = hasArgument(argc, argv, "-e", "--endpoint");

    // parse DNS name
    this->alf_id = getArgument(argc, argv, "-a", "--alf_id");
    // check if set in environmental variables
    if (!this->alf_id)
        this->alf_id = std::getenv("ALF_ID");
    // validate DNS name
    if (!this->alf_id) {
        PrintError("Required parameter \"--alf_id\" is not provided!");
        std::exit(1);
    } else if (!validateDnsName(this->alf_id)) {
        PrintError(
                "Required parameter \"--alf_id\" has invalid value: \"" +
                std::string(this->alf_id) +
                "\"!"
        );
        std::exit(1);
    }
}

bool AlfSimulatorConfig::hasArgument(int argc, char **argv, const char *short_name, const char *name) {
    for (int i = 1; i < argc; i++) {
        if ((short_name && std::strcmp(short_name, argv[i]) == 0) || std::strcmp(name, argv[i]) == 0) {
            return true;
        }
    }

    return false;
}

char *AlfSimulatorConfig::getArgument(int argc, char **argv, const char *short_name, const char *name) {
    for (int i = 1; i < argc; i++) {
        if ((short_name && std::strcmp(short_name, argv[i]) == 0) || std::strcmp(name, argv[i]) == 0) {
            if (i + 1 < argc) {
                return argv[i + 1];
            }
        }
    }

    return nullptr;
}

void AlfSimulatorConfig::print_help() {
    const char *help_string =
            "This is an Alf simulator used to test bandwidth of FRED. "
            "It creates multiple DIM channels with following name structure:\n"
            "Alf DNS path: ALF_{alf_id}/SERIAL_{serial_id}/LINK_{link_id}/...\n\n"
            "Arguments:\n"
            "\t-h, --help : prints this help message\n"
            "\t-v, --verbose : enables verbose logging to stdout\n"
            "\t-e, --endpoint : Changes topics mapping to new version of ALF (ALF_[alf_id]/SERIAL_[serial]/ENDPOINT_[0,1]/LINK_[link]/[service]), default without ENDPOINT_[0,1]\n"
            "\t-a, --alf_id <alf_id> : specify ALF DNS name; alternatively it can be set using ALF_ID environmental variable\n"
            "\t[-s, --serials <serials>] : specify how many serial interfaces to create (default: 3)\n"
            "\t[-l, --links <links>] : specify how many links to create per single serial interface (default: 5)\n"
            "\t[-d, --delay <delay>] : specify ALF processing delay in milliseconds (default: 0)\n";

    std::cout << help_string << std::endl;
}

bool AlfSimulatorConfig::validateDnsName(const char *dns_name) {
    if (!dns_name)
        return false;

    while (*dns_name) {
        if (!(std::isalnum(*dns_name) || *dns_name == '_' || *dns_name == '-'))
            return false;
        dns_name++;
    }

    return true;
}

bool AlfSimulatorConfig::parseUnsignedInteger(const char *string, unsigned int *value) {
    if (!string || !value)
        return false;

    // validate length
    std::size_t len = std::strlen(string);
    if (len == 0 || len > 10)
        return false;

    // validate characters
    for (const char *i = string; *i; i++) {
        if (!std::isdigit(*i))
            return false;
    }

    // parse number
    *value = (unsigned int) std::strtoul(string, nullptr, 10);

    return true;
}

unsigned int AlfSimulatorConfig::getUnsignedInt(int argc, char **argv, const char *short_name, const char *name,
                                                unsigned int default_) {
    if (!hasArgument(argc, argv, short_name, name))
        return default_;

    char *value = getArgument(argc, argv, short_name, name);
    if (!value) {
        PrintWarning(std::string("Value for argument \"") + name + std::string("\" is not provided! Using default."));
        return default_;
    }

    if (!parseUnsignedInteger(value, &default_)) {
        PrintWarning(
                std::string("Value \"") + std::string(value) + std::string("\" for argument \"") +
                name + std::string("\" is not valid! Using default.")
        );
        return default_;
    }

    return default_;
}

bool AlfSimulatorConfig::isVerbose() {
    return this->verbose;
}

bool AlfSimulatorConfig::isEndpoint()
{
    return this->endpoint;
}

char *AlfSimulatorConfig::getAlfId() {
    return this->alf_id;
}

unsigned int AlfSimulatorConfig::getLinkCount() {
    return this->link_count;
}

unsigned int AlfSimulatorConfig::getSerialCount() {
    return this->serial_count;
}

unsigned int AlfSimulatorConfig::getResponseDelay() {
    return this->response_delay;
}

