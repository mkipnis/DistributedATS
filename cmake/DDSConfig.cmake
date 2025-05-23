set(DDS_INCLUDE_DIRS "${DDS_ROOT_DIR}/include")
set(DDS_LIBRARY_DIRS "${DDS_ROOT_DIR}/lib")

include(ExternalProject)

if(NOT EXISTS "${DDS_INCLUDE_DIRS}/fastdds/config.hpp")
    message(STATUS "FastDDS not found, building from source...")

    set(DDS_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/dds)

    include(ExternalProject)

    ExternalProject_Add(ASIO
	GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    	GIT_TAG asio-1-28-0
    	UPDATE_DISCONNECTED TRUE
    	SOURCE_SUBDIR asio
	INSTALL_DIR ${DDS_INSTALL_PREFIX}
    	CONFIGURE_COMMAND cd <SOURCE_DIR>/<SOURCE_SUBDIR> && ./autogen.sh && ./configure --prefix=${DDS_INSTALL_PREFIX} --exec-prefix=${DDS_INSTALL_PREFIX} --without-boost
    	BUILD_COMMAND cd <SOURCE_DIR>/<SOURCE_SUBDIR> && make
    	INSTALL_COMMAND cd <SOURCE_DIR>/<SOURCE_SUBDIR> && make install
    )

    ExternalProject_Add(FoonathanMemory
        GIT_REPOSITORY https://github.com/foonathan/memory.git
        GIT_TAG main 
        UPDATE_DISCONNECTED TRUE
	INSTALL_DIR ${DDS_INSTALL_PREFIX}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${DDS_INSTALL_PREFIX}
	    -DBUILD_SHARED_LIBS=ON
    )

    ExternalProject_Add(FastCDR
        GIT_REPOSITORY https://github.com/eProsima/Fast-CDR.git
        GIT_TAG master
        UPDATE_DISCONNECTED TRUE
	DEPENDS FoonathanMemory ASIO
	INSTALL_DIR ${DDS_INSTALL_PREFIX}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${DDS_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=ON
    )

    ExternalProject_Add(FastDDS
        GIT_REPOSITORY https://github.com/eProsima/Fast-DDS.git
        GIT_TAG master
        UPDATE_DISCONNECTED TRUE
	DEPENDS FastCDR
	INSTALL_DIR ${DDS_INSTALL_PREFIX}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${DDS_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=ON
    )
    

endif()

set(DDS_FOUND TRUE)
