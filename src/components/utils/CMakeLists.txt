include_directories (
  ./include
  ../config_profile/include
  ../media_manager/include/
  ../protocol_handler/include/
)

set (SOURCES
    ./src/conditional_variable_posix.cc
    ./src/file_system.cc
    ./src/threads/posix_thread.cc
    ./src/threads/thread_manager.cc
    ./src/threads/thread_validator.cc
    ./src/lock_posix.cc
    ./src/date_time.cc
    ./src/signals_linux.cc
)

add_library("Utils" ${SOURCES})
target_link_libraries ("Utils" log4cxx rt)
