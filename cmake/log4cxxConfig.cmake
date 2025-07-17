set(LOG4CXX_INCLUDE_DIRS "${LOG4CXX_ROOT_DIR}/include")
set(LOG4CXX_LIBRARY_DIRS "${LOG4CXX_ROOT_DIR}/lib")

include(ExternalProject)

if(NOT EXISTS "${LOG4CXX_INCLUDE_DIRS}/log4cxx/log4cxx.h")
    message(STATUS "Log4cxx not found, building from source...")

    set(LOG4CXX_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/log4cxx)

    include(ExternalProject)

    ExternalProject_Add(Log4cxx
        GIT_REPOSITORY https://github.com/apache/logging-log4cxx.git
        GIT_TAG master
        UPDATE_DISCONNECTED TRUE
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${LOG4CXX_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=ON
    )

endif()

set(Log4cxx_FOUND TRUE)
