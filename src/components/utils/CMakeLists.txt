# Copyright (c) 2016, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/platform.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

find_package(SDLSqlite3 REQUIRED)

include_directories(
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/transport_manager/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${SQLITE3_INCLUDE_DIRECTORY}
)

# dbms
set(DBMS_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/utils/sqlite_wrapper
  ${CMAKE_CURRENT_SOURCE_DIR}/src/sqlite_wrapper
)

set(EXCLUDE_PATHS
  qdb_wrapper
  ${DBMS_PATHS}
)

set(LIBRARIES
  config_profile
  jsoncpp
  ${SQLITE3_LIBRARIES}
)

get_sdk(SDK)
if(SDK)
  if(${SDK} STREQUAL "qt")
    find_package(Qt5 REQUIRED Core Network Sql)

    # Files copied for the Qt MOC.
    # Because MOC is looking headre nearby with *.cc
    # This headers include in .git exception. see .gitignore
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${COMPONENTS_DIR}/include/utils/threads/thread.h
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/thread.h)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${COMPONENTS_DIR}/include/utils/threads/thread_delegate.h
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/thread_delegate.h)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils/socket.h
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/socket.h)

    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    qt5_generate_moc(qthread.cc moc_thread.cpp)
    qt5_generate_moc(socket_qt.cc moc_socket.cpp)
    qt5_generate_moc(thread_delegate_qt.cc moc_thread_delegate.cpp)

    set(DBMS_PATHS
      ${CMAKE_CURRENT_SOURCE_DIR}/include/utils/sql_qt_wrapper
      ${CMAKE_CURRENT_SOURCE_DIR}/src/sql_qt_wrapper
    )
    list(APPEND EXCLUDE_PATHS
      conditional_variable_win.cc
      lock_win.cc
      shared_library_win.cc
      thread_win.cc
      thread_delegate_win.cc
      logger_win.cc
      log_message_loop_thread_win.cc
      json_utils.cc
    )
    list(APPEND LIBRARIES
      Qt5::Core
      Qt5::Network
      Qt5::Sql
    )
  endif()
endif()

collect_sources(DBMS_SOURCES "${DBMS_PATHS}")

add_library(dbms ${DBMS_SOURCES})
target_link_libraries(dbms ${LIBRARIES})

if(ENABLE_LOG)
  if(LOG4CXX_LOGGER)
    list(APPEND LIBRARIES
      log4cxx -L${LOG4CXX_LIBS_DIRECTORY}
      apr-1 -L${APR_LIBS_DIRECTORY}
      aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY}
    )
  endif()
else()
  list(APPEND EXCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log
  )
endif()

if(NOT BUILD_BACKTRACE_SUPPORT)
  list(APPEND EXCLUDE_PATHS
    back_trace.cc
  )
endif()

if(NOT THREAD_COUNT)
  list(APPEND EXCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/thread_counter.cc
  )
endif()

if(NOT BUILD_BT_SUPPORT)
  list(APPEND EXCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils/bluetooth
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bluetooth
  )
endif()

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

get_os(OS)
if(${OS} STREQUAL "posix")
  list(APPEND LIBRARIES
    pthread
    ${RTLIB}
    dl
  )
else(${OS} STREQUAL "win")
  list(APPEND EXCLUDE_PATHS
    appenders_loader.cc
    resource_usage.cc
  )
  list(APPEND LIBRARIES
    ws2_32
  )
endif()

collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

add_library(utils ${SOURCES})
target_link_libraries(utils ${LIBRARIES})

if(LOG4CXX_LOGGER)
  add_dependencies(utils install-3rd_party_logger)
endif()

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
