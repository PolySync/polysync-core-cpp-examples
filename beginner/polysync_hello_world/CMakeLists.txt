project( polysync_hello_world )

cmake_minimum_required( VERSION 2.8 )

include(
    /usr/local/polysync/build_res.cmake
)
include_directories(
    ${PSYNC_INCLUDE_DIRS}
)
set( CPP_FILES
    main.cpp
)
add_executable(
    ${PROJECT_NAME}
    ${CPP_FILES}
)
target_link_libraries(
    ${PROJECT_NAME}
    ${PSYNC_LIBS}
)
