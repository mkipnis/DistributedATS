set(QUICKFIX_INCLUDE_DIR "${QUICKFIX_ROOT_DIR}/include")
set(QUICKFIX_LIBRARY_DIR "${QUICKFIX_ROOT_DIR}/lib")

include(ExternalProject)

if(NOT EXISTS "${QUICKFIX_INCLUDE_DIR}/quickfix/config-all.h")
    message(STATUS "Quickfix not found, building from source...")

    set(QUICKFIX_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/quickfix)

    include(ExternalProject)

    ExternalProject_Add(QuickFIX
        GIT_REPOSITORY https://github.com/quickfix/quickfix.git
        GIT_TAG master
        UPDATE_DISCONNECTED TRUE
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${QUICKFIX_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=ON
    )

endif()

set(quickfix_FOUND TRUE)
