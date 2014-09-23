set(UtilsIncludeDir ${CMAKE_SOURCE_DIR}/src/components/utils/include)

include_directories (
  ./include
  ../config_profile/include
  ../media_manager/include/
  ../protocol_handler/include/
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/bitstream.cc
    ./src/conditional_variable_posix.cc
    ./src/file_system.cc
    ./src/threads/posix_thread.cc
    ./src/threads/thread_manager.cc
    ./src/threads/thread_validator.cc
    ./src/lock_posix.cc
    ./src/rwlock_posix.cc
    ./src/date_time.cc
    ./src/signals_linux.cc
    ./src/system.cc
    ./src/resource_usage.cc
    ./src/push_log.cc
    ./src/log_message_loop_thread.cc
    ./src/logger_status.cc
)

if (BUILD_BACKTRACE_SUPPORT)
    list(APPEND SOURCES
    ./src/back_trace.cc
    )
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
    list(APPEND SOURCES
    ./src/threads/pulse_thread_delegate.cc
    )
endif()

add_library("Utils" ${SOURCES})

if(ENABLE_LOG)
  target_link_libraries("Utils" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries("Utils" pthread ${RTLIB})
endif()
