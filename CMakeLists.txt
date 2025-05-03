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

if(NOT fastdds_FOUND)
    find_package(fastdds 3 REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
install(CODE "
    file(GLOB_RECURSE SCRIPT_FILES
        \"\${CMAKE_INSTALL_PREFIX}/MiscATS/*.sh\")
    foreach(script \${SCRIPT_FILES})
        execute_process(COMMAND chmod +x \${script})
    endforeach()
")
install(FILES 	FIXGateway/scripts/fixgateway.sh 
		DataService/scripts/dataservice.sh 
		MatchingEngine/scripts/matchingengine.sh 
		DESTINATION scripts
   		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE)

install(FILES 	config/log4cxx.xml DESTINATION config)
install(FILES 	FIXGateway/spec/FIX44.xml DESTINATION spec)
