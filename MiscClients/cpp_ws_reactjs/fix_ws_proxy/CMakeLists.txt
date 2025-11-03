cmake_minimum_required(VERSION 3.12.4)

project(fix_ws_proxy)

message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

message(QUICKFIX_INCLUDE_DIR="${QUICKFIX_INCLUDE_DIR}")
message(QUICKFIX_LIBRARY_DIR="${QUICKFIX_LIBRARY_DIR}")

include_directories(${QUICKFIX_INCLUDE_DIR})
link_directories(${QUICKFIX_LIBRARY_DIR})

include_directories(${LOG4CXX_INCLUDE_DIR})
link_directories(${LOG4CXX_LIBRARY_DIR})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB FIX_WS_SRC *.cpp)
add_executable(fix_ws_proxy ${FIX_WS_SRC})

find_package(Boost REQUIRED COMPONENTS program_options thread chrono json)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(fix_ws_proxy
    PRIVATE
    quickfix
    log4cxx
    Boost::program_options
    Boost::thread
    Boost::json
    Boost::chrono
)
