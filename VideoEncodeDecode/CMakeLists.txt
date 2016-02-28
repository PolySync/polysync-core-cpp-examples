cmake_minimum_required(
    VERSION 2.8
    )

project(
    polysync-video-encode-decode-cpp
    )

add_definitions(
    -std=c++11
    )

set( PSYNC_HOME
    /usr/local/polysync
    )

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
    VideoEncodeDecode.cpp

    )

target_link_libraries(
    ${PROJECT_NAME}
    polysync++
    polysync_cpp_message_support
    polysync_data_model
    )
