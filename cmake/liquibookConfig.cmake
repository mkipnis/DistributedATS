set(LIQUIBOOK_INCLUDE_DIRS "${LIQUIBOOK_ROOT_DIR}/include")
set(LIQUIBOOK_LIBRARY_DIRS "${LIQUIBOOK_ROOT_DIR}/lib")

include(ExternalProject)

if(NOT EXISTS "${LIQUIBOOK_INCLUDE_DIRS}/liquibook_export.h")
    message(STATUS "Liquibook not found, building from source...")

    set(LIQUIBOOK_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/liquibook)

    include(ExternalProject)

    ExternalProject_Add(LiquiBook
        GIT_REPOSITORY https://github.com/enewhuis/liquibook.git
        GIT_TAG master
        UPDATE_DISCONNECTED TRUE
        INSTALL_DIR ""
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	SOURCE_SUBDIR src
        INSTALL_COMMAND mkdir -p ${CMAKE_BINARY_DIR}/liquibook && cp -r <SOURCE_DIR>/<SOURCE_SUBDIR> ${LIQUIBOOK_INSTALL_PREFIX}/include
    )

endif()

set(liquibook_FOUND TRUE)
