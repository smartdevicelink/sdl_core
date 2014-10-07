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

add_library("Utils" ${SOURCES})

if(ENABLE_LOG)
  target_link_libraries("Utils" log4cxx -L${LOG4CXX_LIBS_DIRECTORY} apr-1 -L${APR_LIBS_DIRECTORY})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries("Utils" pthread ${RTLIB})
endif()

#======================= Unit-Test section ======================= 
if(BUILD_TESTS)
  include_directories (
    ../../3rd_party-static/gmock-1.7.0/include
    ../../3rd_party-static/gmock-1.7.0/gtest/include)
  
  set(testSources
    ./src/main.cc
    ./src/file_system_test.cc
    ./src/date_time_test.cc)

  set(testLibraries
    gmock
    gtest
    Utils)
    
  add_executable(utils_test ${testSources})
  target_link_libraries(utils_test ${testLibraries})
endif()
#=================================================================
