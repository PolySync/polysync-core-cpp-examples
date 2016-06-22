cmake_minimum_required( VERSION 2.8 )

project( polysync-logfile-queue-reader-cpp )

if( NOT PSYNC_HOME )
    if( $ENV{PSYNC_HOME} )
        set( PSYNC_HOME $ENV{PSYNC_HOME} )
    else()
        set( PSYNC_HOME /usr/local/polysync )
    endif()
endif()

include( ${PSYNC_HOME}/BuildResources.cmake )

find_package( PkgConfig REQUIRED )

pkg_check_modules( GLIB_PKG glib-2.0 )

#include_directories(${GLIB_PKG_INCLUDE_DIRS})

include_directories(
    .
    ${PSYNC_INCLUDE_DIRS}
    ${GLIB_PKG_INCLUDE_DIRS}
    )

add_executable( ${PROJECT_NAME}
    PolySyncLogFile.cpp
    LogfileQueueReaderExample.cpp
)

target_link_libraries( ${PROJECT_NAME}
    ${PSYNC_LIBS}
    glib-2.0
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${PSYNC_HOME}/bin
)

