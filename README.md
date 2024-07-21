# ALF Simulator
ALF Simulator is a simple program simulating ALF responses without actual hardware. 
It is used to test throughput of the FRED. 

Following commandline arguments are recognized:

* `-h or --help` : prints this help message
* `-v or --verbose` : enables verbose logging to stdout
* `-a or --alf_id <alf_id>` : specify ALF DNS name; alternatively it can be set using ALF_ID environmental variable
* `-e or --endpoint` : generate topics with ENDPOINT_[0,1] - (ALF_[alf_id]/SERIAL_[serial]/ENDPOINT_[0,1]/LINK_[link]/[service]), default (ALF_[alf_id]/SERIAL_[serial]/LINK_[link]/[service])
* `[-s or --serials <serials>]` : specify how many serial interfaces to create (default: 3)
* `[-l or --links <links>]` : specify how many links to create per single serial interface (default: 5)
* `[-d or --delay <delay>]` : specify Alf processing delay in milliseconds (default: 0)

Example: 
```
export DIM_DNS_NODE=aldcs075.cern.ch
./AlfSimulator -a 42 -s 2 -l 12 -v -d 150
```

## Custom ECHO Handler:

Example for custom handler is located in `src/custom/customhandler.cpp` and `include/custom/customhandler.h` respectively.
For creation of custom handler it is needed to create Class which inherits `RpcCustomHandler` interface. Then you need to register your handler for desired topic in `main.cpp` in function `registerCustomRpc()`. If no custom handler is provided for topic, random messages are returned as response  based on O2 Alf documentation. Default handling of different protocols is managed in `rpc.cpp` in function `handleRequest()`. Your custom handling of incoming messages should look similar to this. 

## Installation

```
git clone https://gitlab.cern.ch/alice-dcs-kosiceteam/alf-simulator
cd alf-simulator
source scl_source enable devtoolset-7
cmake3 .
make all
```

# Credits
Project is based on [Michele Donadoni code](https://gitlab.cern.ch/mdonadon/ALFDummy).
