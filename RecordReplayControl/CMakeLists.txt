project( RecordReplayExample )

cmake_minimum_required( VERSION 2.8 )

find_package( Threads REQUIRED )

add_definitions( -std=c++11 )

set(POLYSYNC_ROOT ../../..)
set(CPP_API_ROOT ${POLYSYNC_ROOT}/cpp_api)

include_directories(
    ${POLYSYNC_ROOT}/include
    ${POLYSYNC_ROOT}/pdm
    ${CPP_API_ROOT}/include
    ${POLYSYNC_ROOT}/modules/dds/x86_64.linux/include/sys/
)

add_executable( RecordReplayControl
    src/RecordReplayControl.cpp
)

add_executable( AsyncRecordReplayControl
    src/AsyncRecordReplayControl.cpp
)

find_library( PolySync
    polysync
)

find_library( PolySync++
    polysync++
)

find_library( PolySyncDataModel
    polysync_data_model
)

find_library( PolySync++MessageSupport
    polysync_cpp_message_support
)

target_link_libraries(
    RecordReplayControl
    ${PolySync}
    ${PolySync++}
    ${PolySyncDataModel}
    ${PolySync++MessageSupport}
)

target_link_libraries(
    AsyncRecordReplayControl
    ${PolySync}
    ${PolySync++}
    ${PolySyncDataModel}
    ${PolySync++MessageSupport}
)
