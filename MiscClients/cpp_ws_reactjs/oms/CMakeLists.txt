cmake_minimum_required(VERSION 3.12.4)

project(oms)

message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

message(QUICKFIX_INCLUDE_DIR="${QUICKFIX_INCLUDE_DIR}")
message(QUICKFIX_LIBRARY_DIR="${QUICKFIX_LIBRARY_DIR}")

include_directories(${QUICKFIX_INCLUDE_DIR})
link_directories(${QUICKFIX_LIBRARY_DIR})

include_directories(${LOG4CXX_INCLUDE_DIR})
link_directories(${LOG4CXX_LIBRARY_DIR})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB OMS_SRC *.cpp)
add_executable(oms ${OMS_SRC})

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(oms
    PRIVATE
    quickfix
    log4cxx
    Boost::program_options
)
