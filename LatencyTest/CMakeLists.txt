cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(LatencyTest)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

find_package(log4cxx REQUIRED)

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
        add_compile_options(-std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

file(GLOB DISTRIBUTED_ATS_LATENCY_TEST_SRC *)
add_executable(LatencyTest ${DISTRIBUTED_ATS_LATENCY_TEST_SRC})
link_directories(DistributedATSLib quickfix log4cxx)
target_link_libraries(LatencyTest DistributedATSLib quickfix log4cxx boost_program_options)
