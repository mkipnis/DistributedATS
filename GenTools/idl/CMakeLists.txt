cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(DistributedATSLib)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastdds_FOUND)
    find_package(fastdds 3 REQUIRED)
endif()

find_package(log4cxx REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

file(GLOB DISTRIBUTED_ATS_LIB *)
include_directories(${CMAKE_INSTALL_PREFIX}/include)
add_library(DistributedATSLib SHARED ${DISTRIBUTED_ATS_LIB})
target_link_libraries(DistributedATSLib quickfix fastcdr fastdds log4cxx)

install(TARGETS DistributedATSLib
        PUBLIC_HEADER
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dats
)
