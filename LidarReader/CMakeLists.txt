project( LidarReader )

cmake_minimum_required( VERSION 2.8 )

include ( /usr/local/polysync/BuildResources.cmake )

aux_source_directory( . SRC_LIST )

include_directories(
    ${PSYNC_INCLUDE_DIRS}
)

add_executable( ${PROJECT_NAME}
    ${SRC_LIST}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${PSYNC_LIBS}
)
