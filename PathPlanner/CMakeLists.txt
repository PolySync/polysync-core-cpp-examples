cmake_minimum_required( VERSION 2.8 )

project( polysync-path-planner-cpp )

if( NOT PSYNC_HOME )
    if( $ENV{PSYNC_HOME} )
        set( PSYNC_HOME $ENV{PSYNC_HOME} )
    else()
        set( PSYNC_HOME /usr/local/polysync )
    endif()
endif()

find_package( OpenCV REQUIRED )

include( ${PSYNC_HOME}/BuildResources.cmake )

include_directories(
    ${PSYNC_INCLUDE_DIRS}
)

aux_source_directory( . SRC )

add_executable( ${PROJECT_NAME}
    #${SRC}
    #PathPlanner.cpp
    GridMap.cpp
		search.cpp
)

target_link_libraries( ${PROJECT_NAME}
    ${PSYNC_LIBS}
    ${OpenCV_LIBS}
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${PSYNC_HOME}/bin
    )
