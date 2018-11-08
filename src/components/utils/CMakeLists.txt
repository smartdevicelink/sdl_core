project(utils)

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src/appenders_loader.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/bitstream.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/conditional_variable_boost.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/convert_utils.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/custom_string.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/date_time.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/file_system.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/gen_hash.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/lock_boost.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/resource_usage.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/rwlock_posix.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/signals_posix.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/system_time_handler.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/system.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/timer.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/threads/async_runner.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/threads/thread_delegate.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/threads/thread_posix.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/threads/thread_validator.cc"
)

if(BUILD_BACKTRACE_SUPPORT AND NOT QNXNTO)
  target_sources(${PROJECT_NAME}
    PRIVATE
      "${CMAKE_CURRENT_LIST_DIR}/src/back_trace.cc"
  )
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      BACKTRACE_SUPPORT
  )
endif()

if(QNXNTO)
  target_sources(${PROJECT_NAME}
    PRIVATE
      "${CMAKE_CURRENT_LIST_DIR}/src/threads/pulse_thread_delegate.cc"
  )
endif()

if(ENABLE_LOG)
  SDL_find_package(apr REQUIRED)
  SDL_find_package(apr-util REQUIRED)
  SDL_find_package(log4cxx REQUIRED)
  SDL_find_package(boost REQUIRED)

  find_package(EXPAT REQUIRED)

  target_sources(${PROJECT_NAME}
    PRIVATE
      "${CMAKE_CURRENT_LIST_DIR}/src/logger.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/push_log.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/log_message_loop_thread.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/logger_status.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/auto_trace.cc"
  )
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      apr::apr
      apr-util::apr-util
      log4cxx::log4cxx
      expat
      config_profile::config_profile
      boost::thread
  )
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      ENABLE_LOG
  )
endif()

if(UNIX AND NOT QNXNTO)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      dl
      pthread
      rt
  )
endif()

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    components::interface
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/utils"
    "${CMAKE_SOURCE_DIR}/src"
)

if(QNXNTO)
  add_subdirectory(src/qdb_wrapper)
else()
  add_subdirectory(src/sqlite_wrapper)
endif()
