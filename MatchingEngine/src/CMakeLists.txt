cmake_minimum_required(VERSION 3.12.4)

message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(DDS_INCLUDE_DIR="${DDS_INCLUDE_DIR}")
message(DDS_LIBRARY_DIR="${DDS_LIBRARY_DIR}")

message(QUICKFIX_INCLUDE_DIR="${QUICKFIX_INCLUDE_DIR}")
message(QUICKFIX_LIBRARY_DIR="${QUICKFIX_LIBRARY_DIR}")

message(LIQUIBOOK_LIBRARY_DIR="${LIQUIBOOK_LIBRARY_DIR}")

include_directories(${DDS_INCLUDE_DIR})
link_directories(${DDS_LIBRARY_DIR})

include_directories(${QUICKFIX_INCLUDE_DIR})
link_directories(${QUICKFIX_LIBRARY_DIR})

include_directories(${LOG4CXX_INCLUDE_DIR})
link_directories(${LOG4CXX_LIBRARY_DIR})

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(${LIQUIBOOK_INCLUDE_DIR})

file(GLOB DISTRIBUTED_ATS_MATCHING_ENGINE_SRC *.cpp)

add_executable(MatchingEngine ${DISTRIBUTED_ATS_MATCHING_ENGINE_SRC})

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(MatchingEngine
    PRIVATE
    DistributedATSLib
    quickfix
    log4cxx
    Boost::program_options
    fastcdr
    fastdds
)

set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")

install(TARGETS MatchingEngine
        PUBLIC_HEADER
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dats
)

