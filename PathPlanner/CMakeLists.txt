cmake_minimum_required( VERSION 2.8 )

set( CMAKE_BUILD_TYPE "Debug" )

project( polysync-path-planner-algorithm-cpp )

project( polysync-path-planner-robot-cpp )

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
	include
)

add_executable( polysync-path-planner-algorithm-cpp
	source/GridMap.cpp
	source/Planner.cpp
	SearchNode.cpp
)

target_link_libraries( polysync-path-planner-algorithm-cpp
    ${PSYNC_LIBS}
    ${OpenCV_LIBS}
)

add_executable( polysync-path-planner-robot-cpp
	source/GridMap.cpp
	RobotNode.cpp
)

target_link_libraries( polysync-path-planner-robot-cpp
    ${PSYNC_LIBS}
    ${OpenCV_LIBS}
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${PSYNC_HOME}/bin
)
