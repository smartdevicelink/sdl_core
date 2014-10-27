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
    ./src/appenders_loader.cc
)

if(ENABLE_LOG)
  list(APPEND SOURCES
    ./src/push_log.cc
    ./src/log_message_loop_thread.cc
    ./src/logger_status.cc
  )
endif()

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

add_library("Utils" SHARED ${SOURCES})

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
  list(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
  list(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
  target_link_libraries("Utils" ${LIBRARIES})
  ADD_DEPENDENCIES(Utils install-3rd_party_logger)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries("Utils" pthread ${RTLIB})
endif()

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

install(TARGETS "Utils"
  DESTINATION bin
  PERMISSIONS
    OWNER_READ OWNER_WRITE
    GROUP_READ
    WORLD_READ
)
