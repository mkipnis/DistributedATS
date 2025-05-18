cmake_minimum_required(VERSION 3.28.3)

project(DistributedATS)

set(CMAKE_PREFIX_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package( Boost )
find_package( DDS REQUIRED )
find_package( log4cxx REQUIRED )
find_package( quickfix REQUIRED )
find_package( liquibook REQUIRED )

set(DDS_INCLUDE_DIR "${DDS_INSTALL_PREFIX}/include")
set(DDS_LIBRARY_DIR "${DDS_INSTALL_PREFIX}/lib")
set(QUICKFIX_INCLUDE_DIR "${QUICKFIX_INSTALL_PREFIX}/include")
set(QUICKFIX_LIBRARY_DIR "${QUICKFIX_INSTALL_PREFIX}/lib")
set(LOG4CXX_INCLUDE_DIR "${LOG4CXX_INSTALL_PREFIX}/include")
set(LOG4CXX_LIBRARY_DIR "${LOG4CXX_INSTALL_PREFIX}/lib")

set(LIQUIBOOK_INCLUDE_DIR "${LIQUIBOOK_INSTALL_PREFIX}/include")

include_directories( Common )
add_subdirectory( GenTools/idl )

include_directories( GenTools/idl )
add_subdirectory( FIXGateway/src )
add_subdirectory( DataService/src )
add_subdirectory( MatchingEngine/src )
add_subdirectory( LatencyTest )

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

install( FILES 	config/log4cxx.xml DESTINATION config )
install( FILES 	FIXGateway/spec/FIX44.xml DESTINATION spec )
