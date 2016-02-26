cmake_minimum_required( VERSION 2.8.11 )

project( polysync-helloworld-publisher-cpp )

add_definitions( -std=c++11 )

set( PSYNC_HOME
    /usr/local/polysync
    )

include_directories(
    ${PSYNC_HOME}/include
    ${PSYNC_HOME}/pdm
    ${PSYNC_HOME}/include/deps
    ${PSYNC_HOME}/include/deps/dcps/C/SAC
    ${PSYNC_HOME}/include/deps/sys
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
    )


link_directories(
    ${PSYNC_HOME}/lib
    )

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    polysync++
    polysync_cpp_message_support
    polysync_data_model
)
