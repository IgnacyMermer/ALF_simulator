cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
project(AlfSimulator)

include_directories(include/AlfSimulator include)
include_directories(include/custom include)

#find_package(Qt5 REQUIRED COMPONENTS Core Network)

find_package(Boost REQUIRED COMPONENTS system thread)

include_directories(include/DimRpcParallel include ${Boost_INCLUDE_DIRS})
link_directories(lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

file(GLOB_RECURSE SOURCES "src/*.cpp"
                          "src/custom/*.cpp"
)


add_executable(AlfSimulator ${SOURCES})

#target_link_libraries(AlfSimulator Qt5::Core Qt5::Network)
target_link_libraries(AlfSimulator libdim.a libpthread.so ${Boost_LIBRARIES})

