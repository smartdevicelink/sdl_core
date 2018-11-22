# Copyright (c) 2018, Ford Motor Company
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

SDL_find_package(apr-util)

if(TARGET apr-util::apr-util)
  return()
endif()

set(FORCE_APR_BUILD ON PARENT_SCOPE)

set(APR_UTIL_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../apr-util-1.5.3)
set(APR_UTIL_BUILD_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../apr-util-build)

set(APR_UTIL_INCLUDE_DIRECTORY ${APR_UTIL_SOURCE_DIRECTORY}/include)
set(APR_UTIL_LIBS_DIRECTORY ${APR_UTIL_BUILD_DIRECTORY}/.libs)

file(MAKE_DIRECTORY
  ${APR_UTIL_BUILD_DIRECTORY}
)

set(EXPAT_SOURCE_DIRECTORY ${APR_UTIL_SOURCE_DIRECTORY}/../expat-2.1.0)
set(EXPAT_BUILD_DIRECTORY ${APR_UTIL_BUILD_DIRECTORY}/../expat-2.1.0)

include("./${PLATFORM}.cmake")

add_custom_command(OUTPUT ${APR_UTIL_BUILD_DIRECTORY}/Makefile
                   COMMAND CC=${CMAKE_C_COMPILER}
                           ${APR_UTIL_SOURCE_DIRECTORY}/configure
                           ${CONFIGURE_FLAGS}
                   DEPENDS libapr-1
                   DEPENDS expat
                   WORKING_DIRECTORY ${APR_UTIL_BUILD_DIRECTORY})

add_library(apr-util::apr-util UNKNOWN IMPORTED GLOBAL)
set_target_properties(apr-util::apr-util
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${APR_UTIL_INCLUDE_DIRECTORY}"
    IMPORTED_LOCATION "${APR_UTIL_LIBS_DIRECTORY}/libaprutil-1.so"
)

add_custom_target(apr-util ALL make
                  DEPENDS ${APR_UTIL_BUILD_DIRECTORY}/Makefile
                  WORKING_DIRECTORY ${APR_UTIL_BUILD_DIRECTORY})

install(DIRECTORY ${APR_UTIL_LIBS_DIRECTORY}/
        DESTINATION ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib
        USE_SOURCE_PERMISSIONS
        FILES_MATCHING
        PATTERN libaprutil-1.so*)

install(DIRECTORY ${APR_UTIL_INCLUDE_DIRECTORY}/
        DESTINATION ${3RD_PARTY_INSTALL_PREFIX}/include
        USE_SOURCE_PERMISSIONS
        FILES_MATCHING
        PATTERN *.h)

install(DIRECTORY ${APR_UTIL_BUILD_DIRECTORY}/include/
        DESTINATION ${3RD_PARTY_INSTALL_PREFIX}/include
        USE_SOURCE_PERMISSIONS
        FILES_MATCHING
        PATTERN *.h)
