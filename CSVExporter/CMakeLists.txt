cmake_minimum_required( VERSION 2.8 )

project( polysync-csv-export )

if( NOT PSYNC_HOME )
    if( $ENV{PSYNC_HOME} )
        set( PSYNC_HOME $ENV{PSYNC_HOME} )
    else()
        set( PSYNC_HOME /usr/local/polysync )
    endif()
endif()

include( ${PSYNC_HOME}/BuildResources.cmake )

include_directories(
    include
    ${PSYNC_INCLUDE_DIRS}
)

add_executable( ${PROJECT_NAME}
    CSVExport.cpp
    SubscriberMap.cpp
    Subscriber.cpp
)

target_link_libraries( ${PROJECT_NAME}
    getopt_cpp 
    ${PSYNC_LIBS}
)
