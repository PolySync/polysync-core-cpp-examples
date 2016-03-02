cmake_minimum_required(
    VERSION 2.8.11
    )

project(
    polysync-video-device-viewer-c++
    )

add_definitions(
    -std=c++11
    )

set( PSYNC_HOME
    /usr/local/polysync
    )

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )

find_package( Qt5Widgets )

include_directories(
    ${PSYNC_HOME}/include
    ${PSYNC_HOME}/pdm
    ${PSYNC_HOME}/include/deps/dcps/C/SAC
    ${PSYNC_HOME}/include/deps/sys
    )

link_directories(
    ${PSYNC_HOME}/lib
    )

add_executable(
    ${PROJECT_NAME}
    main.cpp
    VideoViewer.cpp
    VideoProcessor.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}
    polysync++
    polysync_cpp_message_support
    polysync_data_model
    Qt5::Widgets
    )
