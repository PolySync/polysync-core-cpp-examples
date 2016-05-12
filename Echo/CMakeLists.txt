cmake_minimum_required( VERSION 2.8 )

project( polysync-echo )

if( NOT PSYNC_HOME )
    if( $ENV{PSYNC_HOME} )
        set( PSYNC_HOME $ENV{PSYNC_HOME} )
    else()
        set( PSYNC_HOME /usr/local/polysync )
    endif()
endif()

include( ${PSYNC_HOME}/BuildResources.cmake )

include_directories(
    .
    ${PSYNC_HOME}/include
    ${PSYNC_HOME}/pdm
    ${PSYNC_HOME}/include/deps/dcps/C/SAC
    ${PSYNC_HOME}/include/deps/sys
    )

add_executable( ${PROJECT_NAME}
    Echo.cpp
    EchoNode.cpp
    ApplicationInputHandler.cpp
    EchoHelp.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}
    polysync++
    polysync_cpp_message_support
    polysync_data_model
    )

install(
    TARGETS polysync-echo
    DESTINATION ${PSYNC_HOME}/bin
)
