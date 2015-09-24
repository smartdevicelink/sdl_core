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

include_directories (
  ${UTILS_INCLUDE_DIR}
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/media_manager/include
  ${COMPONENTS_DIR}/protocol_handler/include
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ${UTILS_SRC_DIR}/bitstream.cc
    ${UTILS_SRC_DIR}/conditional_variable_posix.cc
    ${UTILS_SRC_DIR}/file_system.cc
    ${UTILS_SRC_DIR}/threads/posix_thread.cc   
    ${UTILS_SRC_DIR}/threads/thread_delegate.cc
    ${UTILS_SRC_DIR}/threads/thread_validator.cc
    ${UTILS_SRC_DIR}/threads/async_runner.cc
    ${UTILS_SRC_DIR}/lock_posix.cc
    ${UTILS_SRC_DIR}/rwlock_posix.cc
    ${UTILS_SRC_DIR}/date_time.cc
    ${UTILS_SRC_DIR}/signals_linux.cc
    ${UTILS_SRC_DIR}/system.cc
    ${UTILS_SRC_DIR}/resource_usage.cc
    ${UTILS_SRC_DIR}/appenders_loader.cc
    ${UTILS_SRC_DIR}/gen_hash.cc
)

if(ENABLE_LOG)
  list(APPEND SOURCES
    ${UTILS_SRC_DIR}/push_log.cc
    ${UTILS_SRC_DIR}/log_message_loop_thread.cc
    ${UTILS_SRC_DIR}/logger_status.cc
    ${UTILS_SRC_DIR}/auto_trace.cc
    ${UTILS_SRC_DIR}/logger.cc
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

install(TARGETS "Utils"
  DESTINATION bin
  PERMISSIONS
    OWNER_READ OWNER_WRITE
    GROUP_READ
    WORLD_READ
)
