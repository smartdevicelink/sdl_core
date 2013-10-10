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
)

add_library("Utils" ${SOURCES})
target_link_libraries ("Utils" log4cxx rt)
