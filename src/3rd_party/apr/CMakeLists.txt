# Copyright (c) 2018, Ford Motor Company All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

if(NOT FORCE_APR_BUILD)
  sdl_find_package(apr)
endif()

if(TARGET apr::apr)
  set(apr_FOUND ON)
  return()
endif()

set(APR_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../apr-1.5.0)
set(APR_BUILD_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../apr-build)
set(APR_INCLUDE_DIRECTORY ${APR_SOURCE_DIRECTORY}/include)
set(APR_LIBS_DIRECTORY ${APR_BUILD_DIRECTORY}/.libs)

file(MAKE_DIRECTORY ${APR_BUILD_DIRECTORY}/include/private
                    ${APR_SOURCE_DIRECTORY}/include/private)

file(COPY
     ${APR_SOURCE_DIRECTORY}/tools/gen_test_char.c
     DESTINATION
     ${APR_BUILD_DIRECTORY}/tools)

include("./${PLATFORM}.cmake")

add_custom_command(OUTPUT ${APR_BUILD_DIRECTORY}/Makefile
                   COMMAND CC=${CMAKE_C_COMPILER}
                           ${APR_SOURCE_DIRECTORY}/configure ${CONFIGURE_FLAGS}
                   COMMAND ${CMAKE_COMMAND} -E copy include/apr.h
                           ${APR_SOURCE_DIRECTORY}/include
                   WORKING_DIRECTORY ${APR_BUILD_DIRECTORY})

add_custom_command(
  OUTPUT ${APR_BUILD_DIRECTORY}/include/private/apr_escape_test_char.h
  DEPENDS ${APR_BUILD_DIRECTORY}/Makefile
  COMMAND make include/private/apr_escape_test_char.h
  COMMAND ${CMAKE_COMMAND} -E copy include/private/apr_escape_test_char.h
          ${APR_SOURCE_DIRECTORY}/include/private
  WORKING_DIRECTORY ${APR_BUILD_DIRECTORY})

add_library(apr::apr UNKNOWN IMPORTED GLOBAL)
set_target_properties(apr::apr
                      PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                 "${APR_INCLUDE_DIRECTORY}"
                                 IMPORTED_LOCATION
                                 "${APR_LIBS_DIRECTORY}/libapr-1.so")

add_custom_target(
  libapr-1 ALL make
  DEPENDS ${APR_BUILD_DIRECTORY}/include/private/apr_escape_test_char.h
  WORKING_DIRECTORY ${APR_BUILD_DIRECTORY})

install(DIRECTORY ${APR_LIBS_DIRECTORY}/
        DESTINATION ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib
        USE_SOURCE_PERMISSIONS
        FILES_MATCHING
        PATTERN libapr-1.so*)

install(DIRECTORY ${APR_INCLUDE_DIRECTORY}/
        DESTINATION ${3RD_PARTY_INSTALL_PREFIX}/include
        USE_SOURCE_PERMISSIONS
        FILES_MATCHING
        PATTERN *.h)
