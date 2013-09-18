include_directories (
  ./include
  ../config_profile/include
)

set (SOURCES
    ./src/file_system.cc
    ./src/threads/posix_thread.cc
    ./src/synchronisation_primitives_posix.cc
    ./src/timer_linux.cc
    ./src/date_time.cc
    ./src/logger.cc
)

add_library("Utils" ${SOURCES})
