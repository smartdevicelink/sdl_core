# Copyright (c) 2015, Ford Motor Company
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


set(UTILS_INCLUDE_DIR ${COMPONENTS_DIR}/utils/include)
set(UTILS_SRC_DIR ${COMPONENTS_DIR}/utils/src)
set(THREAD_INCLUDE_DIR ${COMPONENTS_DIR}/include/utils/threads/)

include_directories (
  ${UTILS_INCLUDE_DIR}
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/transport_manager/include
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if(QT_PORT)
     find_package(Qt5 REQUIRED Core)
     set ( inc  ${UTILS_INCLUDE_DIR}/utils/bitstream.h
                ${UTILS_INCLUDE_DIR}/utils/file_system.h
                ${UTILS_INCLUDE_DIR}/utils/gen_hash.h
                ${UTILS_INCLUDE_DIR}/utils/helpers.h
                ${UTILS_INCLUDE_DIR}/utils/signals.h
                ${UTILS_INCLUDE_DIR}/utils/singleton.h
                ${UTILS_INCLUDE_DIR}/utils/stl_utils.h
                ${UTILS_INCLUDE_DIR}/utils/string_utils.h
                ${UTILS_INCLUDE_DIR}/utils/system.h
                ${UTILS_INCLUDE_DIR}/utils/json_utils.h
                ${UTILS_INCLUDE_DIR}/utils/host_address.h
                ${UTILS_INCLUDE_DIR}/utils/socket.h
                ${UTILS_INCLUDE_DIR}/utils/socket_utils.h
                ${UTILS_INCLUDE_DIR}/utils/type_traits.h
                ${UTILS_INCLUDE_DIR}/utils/timer.h
                ${UTILS_INCLUDE_DIR}/utils/convert_utils.h)
      set (src ${UTILS_SRC_DIR}/bitstream.cc
           ${UTILS_SRC_DIR}/conditional_variable_qt.cc
            ${UTILS_SRC_DIR}/file_system_qt.cc
            ${UTILS_SRC_DIR}/lock_qt.cc
            ${UTILS_SRC_DIR}/rwlock_qt.cc
            ${UTILS_SRC_DIR}/date_time_qt.cc
            ${UTILS_SRC_DIR}/system.cc
            ${UTILS_SRC_DIR}/signals_qt.cc
            ${UTILS_SRC_DIR}/gen_hash.cc
            ${UTILS_SRC_DIR}/threads/thread_validator.cc
            ${UTILS_SRC_DIR}/threads/thread_qt.cc
            ${UTILS_SRC_DIR}/threads/thread_delegate_qt.cc
            ${UTILS_SRC_DIR}/threads/async_runner.cc
            ${UTILS_SRC_DIR}/string_utils.cc
            ${UTILS_SRC_DIR}/pipe_qt.cc
            ${UTILS_SRC_DIR}/shared_library_qt.cc
            ${UTILS_SRC_DIR}/socket_qt.cc
            ${UTILS_SRC_DIR}/json_utils_qt.cc
            ${UTILS_SRC_DIR}/host_address.cc
            ${UTILS_SRC_DIR}/socket_utils.cc
            ${UTILS_SRC_DIR}/timer.cc
            ${UTILS_SRC_DIR}/custom_string.cc
            ${UTILS_SRC_DIR}/convert_utils.cc)
    # Files copied for the Qt MOC.
    # Because MOC is looking headre nearby with *.cc
    # This headers include in .git exception. see .gitignore
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                  ${THREAD_INCLUDE_DIR}/thread.h
                  ${UTILS_SRC_DIR}/threads/thread.h)
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                  ${THREAD_INCLUDE_DIR}/thread_delegate.h
                  ${UTILS_SRC_DIR}/threads/thread_delegate.h)
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                  ${UTILS_INCLUDE_DIR}/utils/socket.h
                  ${UTILS_SRC_DIR}/socket.h)

    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    qt5_generate_moc( qthread.cc moc_thread.cpp)
    qt5_generate_moc( socket_qt.cc moc_socket.cpp)
    qt5_generate_moc( thread_delegate_qt.cc moc_thread_delegate.cpp)

  else() #WIN_NATIVE
    include_directories (
      ${JSONCPP_INCLUDE_DIRECTORY}
    )
     set ( inc  ${UTILS_INCLUDE_DIR}/utils/bitstream.h
                ${UTILS_INCLUDE_DIR}/utils/file_system.h
                ${UTILS_INCLUDE_DIR}/utils/gen_hash.h
                ${UTILS_INCLUDE_DIR}/utils/helpers.h
                ${UTILS_INCLUDE_DIR}/utils/signals.h
                ${UTILS_INCLUDE_DIR}/utils/singleton.h
                ${UTILS_INCLUDE_DIR}/utils/stl_utils.h
                ${UTILS_INCLUDE_DIR}/utils/string_utils.h
                ${UTILS_INCLUDE_DIR}/utils/system.h
                ${UTILS_INCLUDE_DIR}/utils/json_utils.h
                ${UTILS_INCLUDE_DIR}/utils/host_address.h
                ${UTILS_INCLUDE_DIR}/utils/socket.h
                ${UTILS_INCLUDE_DIR}/utils/socket_utils.h
                ${UTILS_INCLUDE_DIR}/utils/timer.h
                ${UTILS_INCLUDE_DIR}/utils/convert_utils.h)
    set (src
            ${UTILS_SRC_DIR}/bitstream.cc
            ${UTILS_SRC_DIR}/conditional_variable_win.cc
            ${UTILS_SRC_DIR}/date_time_win.cc
            ${UTILS_SRC_DIR}/file_system_win.cc
            ${UTILS_SRC_DIR}/gen_hash.cc
            ${UTILS_SRC_DIR}/lock_win.cc
            ${UTILS_SRC_DIR}/rwlock_win.cc
            ${UTILS_SRC_DIR}/signals_win.cc
            ${UTILS_SRC_DIR}/threads/thread_validator.cc
            ${UTILS_SRC_DIR}/threads/async_runner.cc
            ${UTILS_SRC_DIR}/threads/thread_delegate_win.cc
            ${UTILS_SRC_DIR}/threads/win_thread.cc
            ${UTILS_SRC_DIR}/string_utils.cc
            ${UTILS_SRC_DIR}/pipe_win.cc
            ${UTILS_SRC_DIR}/shared_library_win.cc
            ${UTILS_SRC_DIR}/socket_win.cc
            ${UTILS_SRC_DIR}/json_utils.cc
            ${UTILS_SRC_DIR}/host_address.cc
            ${UTILS_SRC_DIR}/socket_utils.cc
            ${UTILS_SRC_DIR}/timer.cc
            ${UTILS_SRC_DIR}/custom_string.cc
            ${UTILS_SRC_DIR}/convert_utils.cc)
  endif()

  if (BUILD_BT_SUPPORT)
      list(APPEND inc
               ${UTILS_INCLUDE_DIR}/utils/bluetooth_win/bluetooth_utils.h
               ${UTILS_INCLUDE_DIR}/utils/bluetooth_win/bluetooth_uuid.h
               ${UTILS_INCLUDE_DIR}/utils/bluetooth_win/bluetooth_service_record.h)
      list(APPEND src
         ${UTILS_SRC_DIR}/bluetooth_win/bluetooth_utils.cc)
  endif()
  if(ENABLE_LOG)
    list(APPEND LIBRARIES ConfigProfile)
     if(QT_PORT)
       list(APPEND src
         ${UTILS_SRC_DIR}/logger_qt.cc
         ${UTILS_SRC_DIR}/log_message_loop_thread_qt.cc
       )
     else()
        list(APPEND src
          ${UTILS_SRC_DIR}/logger_win.cc
          ${UTILS_SRC_DIR}/log_message_loop_thread_win.cc
        )
     endif()
  endif()
    string( REPLACE / \\ group ${inc} )
    source_group("Header Files" FILES ${inc} )
    string( REPLACE / \\ group ${src} )
    source_group("Source Files" FILES ${src} )

    add_library("Utils" ${src} ${inc})
    if(QT_PORT)
      add_subdirectory(./src/sql_qt_wrapper)
      target_link_libraries("Utils" Qt5::Core Qt5::Network ${LIBRARIES})
    else()
      add_subdirectory(./src/sqlite_wrapper)
      target_link_libraries("Utils" ${LIBRARIES})
    endif()
else()
    include_directories (
      ${JSONCPP_INCLUDE_DIRECTORY}
    )
    set (SOURCES
      ${UTILS_SRC_DIR}/bitstream.cc
      ${UTILS_SRC_DIR}/conditional_variable_posix.cc
      ${UTILS_SRC_DIR}/file_system_posix.cc
      ${UTILS_SRC_DIR}/threads/posix_thread.cc
      ${UTILS_SRC_DIR}/threads/thread_delegate_posix.cc
      ${UTILS_SRC_DIR}/threads/thread_validator.cc
      ${UTILS_SRC_DIR}/threads/async_runner.cc
      ${UTILS_SRC_DIR}/lock_posix.cc
      ${UTILS_SRC_DIR}/rwlock_posix.cc
      ${UTILS_SRC_DIR}/date_time_posix.cc
      ${UTILS_SRC_DIR}/signals_linux.cc
      ${UTILS_SRC_DIR}/system.cc
      ${UTILS_SRC_DIR}/resource_usage.cc
      ${UTILS_SRC_DIR}/appenders_loader.cc
      ${UTILS_SRC_DIR}/gen_hash.cc
      ${UTILS_SRC_DIR}/logger_posix.cc
      ${UTILS_SRC_DIR}/log_message_loop_thread_posix.cc
      ${UTILS_SRC_DIR}/string_utils.cc
      ${UTILS_SRC_DIR}/socket_posix.cc
      ${UTILS_SRC_DIR}/pipe_posix.cc
      ${UTILS_SRC_DIR}/shared_library_posix.cc
      ${UTILS_SRC_DIR}/json_utils.cc
      ${UTILS_SRC_DIR}/host_address.cc
      ${UTILS_SRC_DIR}/socket_utils.cc
      ${UTILS_SRC_DIR}/timer.cc
      ${UTILS_SRC_DIR}/custom_string.cc
      ${UTILS_SRC_DIR}/convert_utils.cc
    )
    if(ENABLE_LOG)
      list(APPEND SOURCES
        ${UTILS_SRC_DIR}/logger_posix.cc
      )
    endif()

    if (BUILD_BACKTRACE_SUPPORT)
        list(APPEND SOURCES
        ${UTILS_SRC_DIR}/back_trace.cc
        )
    endif()

    if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
        list(APPEND SOURCES
        ${UTILS_SRC_DIR}/threads/pulse_thread_delegate.cc
        )
    endif()

    add_library("Utils" ${SOURCES})

    if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
        # --- QDB Wrapper
        add_subdirectory(./src/qdb_wrapper)
    else ()
        # --- SQLite Wrapper
        add_subdirectory(./src/sqlite_wrapper)
    endif ()

    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
     list(APPEND LIBRARIES dl)
    endif()

    if(ENABLE_LOG)
      list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
      list(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
      list(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
      list(APPEND LIBRARIES ConfigProfile)

      ADD_DEPENDENCIES(Utils install-3rd_party_logger)
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
      target_link_libraries("Utils" pthread ${RTLIB})
    endif()

    target_link_libraries("Utils" ${LIBRARIES})


    if(BUILD_TESTS)
      add_subdirectory(test)
    endif()
endif()

    install(TARGETS "Utils"
      DESTINATION bin
      PERMISSIONS
        OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ
    )
