#ifndef ALFSIMULATORCONFIG_H
#define ALFSIMULATORCONFIG_H

#include <cinttypes>

/**
 * Class containing AlfSimulator configuration
 */
class AlfSimulatorConfig {
public:
    /**
     * Constructor parses program arguments
     */
    AlfSimulatorConfig(int argc, char **argv);

    /**
     * Check if verbose flag is set
     * @return true if verbose is set; false otherwise
     */
    bool isVerbose();

    /**
     * Check if endpoint flag is set
     * @return true if endpoint is set; false otherwise
     */
    bool isEndpoint();

    /**
     * Get selected Ald DNS name: ALF_{alf_id}/SERIAL_{serial_id}/LINK_{link_id}/...
     * @return Alf DNS name
     */
    char *getAlfId();

    /**
     * Get selected link count
     * @return link count
     */
    unsigned int getLinkCount();

    /**
     * Get selected serial count
     * @return serial count
     */
    unsigned int getSerialCount();

    /**
     * Get selected "processing" delay
     * @return processing delay
     */
    unsigned int getResponseDelay();

private:
    /**
     * Flag indicating verbose print
     */
    bool verbose;
    /**
     * Alf DNS name: ALF_{alf_id}/SERIAL_{serial_id}/LINK_{link_id}/...
     */
    char *alf_id;
    /**
     * Defines how many links to create per serial interface
     */
    unsigned int link_count;
    /**
     * Defines how many serial interfaces to define
     */
    unsigned int serial_count;
    /**
     * Defines Alf "processing" delay in milliseconds
     */
    unsigned int response_delay;

    /**
     * Defines if ENDPOINT_[0,1] is located in mask (newer version of ALF)
    */

    bool endpoint;

    /**
     * Function prints help and terminates program
     */
    void print_help();

    /**
     * Checks if command line argument is provided (case sensitive)
     * @param argc command line arguments count (including program path)
     * @param argv command line arguments
     * @param short_name argument short name (abbreviation; can be NULL)
     * @param name human readable argument name
     * @return true if argument is provided; false otherwise
     */
    bool hasArgument(int argc, char **argv, const char *short_name, const char *name);

    /**
     * Retrieves argument value or NULL pointer if value is not provided (case sensitive)
     * @param argc command line arguments count (including program path)
     * @param argv command line arguments
     * @param short_name argument short name (abbreviation; used with -; can be NULL)
     * @param name human readable argument name (use with --)
     * @return argument value (or NULL if not provided)
     */
    char *getArgument(int argc, char **argv, const char *short_name, const char *name);

    /**
     * Parses unsigned integer from program arguments (program is terminated with error message in case invalid
     * input is provided).
     * @param argc command line arguments count (including program path)
     * @param argv command line arguments
     * @param short_name argument short name (abbreviation; used with -; can be NULL)
     * @param name human readable argument name (use with --)
     * @param default_ default value returned if argument is not provided
     * @return parsed value or default
     */
    unsigned int getUnsignedInt(int argc, char **argv, const char *short_name, const char *name, unsigned int default_);

    /**
     * Performs simple validation of Alf DNS name
     * @param dns_name Alf DNS name
     * @return true if name is valid; false otherwise
     */
    bool validateDnsName(const char *dns_name);

    /**
     * Tries to parse unsigned integer from provided string
     * @param string string value to be parsed
     * @param value value to be assigned after successful conversion
     * @return true on success; false otherwise
     */
    bool parseUnsignedInteger(const char *string, unsigned int *value);
};

#endif // ALFSIMULATORCONFIG_H
