include_directories (
  ./include
  ../config_profile/include
  ../media_manager/include/
  ../protocol_handler/include/
)

set (SOURCES
    ./src/bitstream.cc
    ./src/conditional_variable_posix.cc
    ./src/file_system.cc
    ./src/threads/posix_thread.cc
    ./src/threads/thread_manager.cc
    ./src/threads/thread_validator.cc
    ./src/lock_posix.cc
    ./src/date_time.cc
    ./src/signals_linux.cc
    ./src/system.cc
    ./src/resource_usage.cc
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

IF(${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  target_link_libraries("Utils" apr-1 aprutil-1)
else()
  target_link_libraries("Utils" apr-1 aprutil-1 rt)
endif()

if(ENABLE_LOG)
  add_dependencies("Utils" liblog4cxx)
  target_link_libraries("Utils" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

