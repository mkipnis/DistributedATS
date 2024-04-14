cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(DistributedATS)
#set(CMAKE_SUPPRESS_REGENERATION true)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11 -g)
    if(SUPPORTS_CXX11)
        add_compile_options(-std=c++11 -g)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
link_directories(${Boost_LIBRARY_DIRS})

link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

include_directories(Common)
add_subdirectory(GenTools/idl)

include_directories(GenTools/idl)
add_subdirectory(FIXGateway/src)
add_subdirectory(DataService/src)
add_subdirectory(MatchingEngine/src)
add_subdirectory(LatencyTest)

set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")

install(DIRECTORY MiscATS DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES 	FIXGateway/scripts/fixgateway.sh 
		DataService/scripts/dataservice.sh 
		MatchingEngine/scripts/matchingengine.sh 
		DESTINATION scripts)

install(FILES 	config/log4cxx.xml DESTINATION config)
install(FILES 	FIXGateway/spec/FIX44.xml DESTINATION spec)
