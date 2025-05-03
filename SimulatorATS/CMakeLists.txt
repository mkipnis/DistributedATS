cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(SimulatorATS)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastdds_FOUND)
    find_package(fastdds 3 REQUIRED)
endif()

find_package(log4cxx REQUIRED)

# Set C++14 instead of C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${CMAKE_QUICKFIX_PREFIX}/lib)
#link_directories(${Boost_LIBRARY_DIRS})

file(GLOB DISTRIBUTED_ATS_SIMULATOR_ATS_SRC *)
add_executable(SimulatorATS ${DISTRIBUTED_ATS_SIMULATOR_ATS_SRC})
link_directories(DistributedATSLib quickfix log4cxx)
target_link_libraries(SimulatorATS DistributedATSLib quickfix log4cxx boost_program_options)

set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")
