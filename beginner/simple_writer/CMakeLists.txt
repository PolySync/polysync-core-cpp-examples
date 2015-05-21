project( simple_writer )

cmake_minimum_required( VERSION 3.0 )

include( /usr/local/polysync/build_res.cmake )

include_directories( ${PSYNC_INCLUDE_DIRS} )

add_executable( ${PROJECT_NAME} main.cpp )

target_link_libraries( ${PROJECT_NAME} ${PSYNC_LIBS} )
