cmake_minimum_required(VERSION 3.12.4)

project(DistributedATSLib)

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

file(GLOB DISTRIBUTED_ATS_LIB *)
add_library(DistributedATSLib SHARED ${DISTRIBUTED_ATS_LIB})
target_link_libraries(DistributedATSLib quickfix fastcdr fastdds log4cxx)

install(TARGETS DistributedATSLib
        PUBLIC_HEADER
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dats
)
