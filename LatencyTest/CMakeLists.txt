cmake_minimum_required(VERSION 3.12.4)

project(LatencyTest)

message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(DDS_INCLUDE_DIR="${DDS_INCLUDE_DIR}")
message(DDS_LIBRARY_DIR="${DDS_LIBRARY_DIR}")

message(QUICKFIX_INCLUDE_DIR="${QUICKFIX_INCLUDE_DIR}")
message(QUICKFIX_LIBRARY_DIR="${QUICKFIX_LIBRARY_DIR}")

include_directories(${DDS_INCLUDE_DIR})
link_directories(${DDS_LIBRARY_DIR})

include_directories(${QUICKFIX_INCLUDE_DIR})
link_directories(${QUICKFIX_LIBRARY_DIR})

include_directories(${LOG4CXX_INCLUDE_DIR})
link_directories(${LOG4CXX_LIBRARY_DIR})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB DISTRIBUTED_ATS_LATENCY_TEST_SRC *.cpp)
add_executable(LatencyTest ${DISTRIBUTED_ATS_LATENCY_TEST_SRC})

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(LatencyTest
    PRIVATE
    DistributedATSLib
    quickfix
    log4cxx
    Boost::program_options
    fastcdr
    fastdds
)
