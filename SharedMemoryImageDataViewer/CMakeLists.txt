cmake_minimum_required( VERSION 2.8 )

project( polysync-sharedmem-image-data-viewer-cpp )

if( NOT PSYNC_HOME )
    if( $ENV{PSYNC_HOME} )
        set( PSYNC_HOME $ENV{PSYNC_HOME} )
    else()
        set( PSYNC_HOME /usr/local/polysync )
    endif()
endif()

include( ${PSYNC_HOME}/BuildResources.cmake )

include_directories(
    ${PSYNC_INCLUDE_DIRS}
)

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )

find_package( Qt5Widgets )

add_executable( ${PROJECT_NAME}
    SharedMemoryImageDataViewer.cpp
    VideoViewer.cpp
    VideoProcessor.cpp
)

target_link_libraries( ${PROJECT_NAME}
    ${PSYNC_LIBS}
    Qt5::Widgets
)
