# Copyright (c) 2014, Ford Motor Company
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

# --- jsoncpp
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

set(JSONCPP_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include ${CMAKE_SOURCE_DIR}/src/components/utils/include)
set(JSONCPP_INCLUDE_DIRECTORY ${JSONCPP_INCLUDE_DIRECTORY} PARENT_SCOPE)

include_directories(
  ${JSONCPP_INCLUDE_DIRECTORY}
)

collect_sources(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/src/lib_json")
add_library(jsoncpp ${SOURCES})

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
# --- sqlite3
set(SQLITE_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sqlite_static ${CMAKE_SOURCE_DIR}/src/components/utils/include)
set(SQLITE_INCLUDE_DIRECTORY ${SQLITE_INCLUDE_DIRECTORY} PARENT_SCOPE)

include_directories(
  ${SQLITE_INCLUDE_DIRECTORY}
)
add_subdirectory(./sqlite_static)
endif ()

if (ANDROID)
  # --- openssl
  include_directories(
          ${OPENSSL_INCLUDE_DIRECTORY}
  )
  add_library(crypto SHARED IMPORTED)
  set_target_properties(crypto
          PROPERTIES
          IMPORTED_LOCATION ${OPENSSL_LIBS_DIRECTORY}/libcrypto.so)

 add_library(ssl SHARED IMPORTED)
  set_target_properties(ssl
          PROPERTIES
          IMPORTED_LOCATION ${OPENSSL_LIBS_DIRECTORY}/libssl.so)
endif()

# --- encryption
add_subdirectory(./encryption)
set(ENCRYPTION_INCLUDE_DIRECTORY ${ENCRYPTION_INCLUDE_DIRECTORY} PARENT_SCOPE)

# --- gmock
if(BUILD_TESTS)
  add_subdirectory(./gmock-1.7.0)
  set(GMOCK_INCLUDE_DIRECTORY ${GMOCK_INCLUDE_DIRECTORY} PARENT_SCOPE)
  add_subdirectory(test)
endif()
