cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(MatchingEngine)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastdds_FOUND)
    find_package(fastdds 3 REQUIRED)
endif()

find_package(log4cxx REQUIRED)

# Set C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${LIQUIBOOK_HOME})
#include_directories(${QUICKFIX_INSTALL_PREFIX}/include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

file(GLOB DISTRIBUTED_ATS_MATCHING_ENGINE_SRC *)
add_executable(MatchingEngine ${DISTRIBUTED_ATS_MATCHING_ENGINE_SRC})
link_directories(DistributedATSLib quickfix log4cxx boost_program_options)
target_link_libraries(MatchingEngine DistributedATSLib quickfix log4cxx boost_program_options)

install(TARGETS MatchingEngine 
        PUBLIC_HEADER
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dats
)
