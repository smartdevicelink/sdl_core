include_directories (
  ./include
  ../config_profile/include
  ../media_manager/include/
  ../protocol_handler/include/
  ${CMAKE_BINARY_DIR}/src/components/
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
)

if (BUILD_BACKTRACE_SUPPORT)
    list(APPEND SOURCES
    ./src/back_trace.cc
    )
endif()

add_library("Utils" ${SOURCES})

IF(${CMAKE_SYSTEM_NAME} MATCHES "QNX")
	target_link_libraries ("Utils" log4cxx apr-1 aprutil-1)
else()
	target_link_libraries ("Utils" log4cxx apr-1 aprutil-1 rt)
endif()
