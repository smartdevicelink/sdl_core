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

set(DBUS_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../dbus-1.7.8)
set(DBUS_BUILD_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../dbus-build)

set(DBUS_BUILD_DIRECTORY ${DBUS_BUILD_DIRECTORY} PARENT_SCOPE)

set(DBUS_INCLUDE_DIR ${3RD_PARTY_INSTALL_PREFIX}/include)
set(DBUS_INCLUDE_DIR_ARCH ${3RD_PARTY_INSTALL_PREFIX_ARCH}/include)
set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIR} ${DBUS_INCLUDE_DIR_ARCH})

set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIRS} PARENT_SCOPE)


file(MAKE_DIRECTORY
  ${DBUS_BUILD_DIRECTORY}
)

set(COMMON_CONFIGURE_FLAGS
  "--enable-tests=no"
  "CFLAGS=-I${EXPAT_INCLUDE_DIRECTORY}"
  "LDFLAGS=-L${EXPAT_LIBS_DIRECTORY}"
)

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  set(CONFIGURE_FLAGS "--host=${CMAKE_SYSTEM_PROCESSOR}-nto-qnx" ${COMMON_CONFIGURE_FLAGS})
else ()
  set(CONFIGURE_FLAGS ${COMMON_CONFIGURE_FLAGS})
endif ()

add_custom_command(OUTPUT ${DBUS_BUILD_DIRECTORY}/Makefile
  COMMAND CC=${CMAKE_C_COMPILER} ${DBUS_SOURCE_DIRECTORY}/configure ${CONFIGURE_FLAGS}
  DEPENDS expat
  WORKING_DIRECTORY ${DBUS_BUILD_DIRECTORY}
)

add_custom_target(dbus ALL make
  COMMAND /bin/bash -c \"grep \\".commit_hash\\" ${DBUS_BUILD_DIRECTORY}/dbus/.libs/libdbus-1.so 1>/dev/null 2>&1\; if [ ! \\\$$? == 0 ]\; then cd ${CMAKE_CURRENT_SOURCE_DIR} && git log --pretty=\\"format:%H\\" -1 ${DBUS_SOURCE_DIRECTORY} > /tmp/commit_hash 2>/dev/null && echo \\"Adding .commit_hash section\\" && ${objcopy} --add-section .commit_hash=/tmp/commit_hash ${DBUS_BUILD_DIRECTORY}/dbus/.libs/libdbus-1.so ${DBUS_BUILD_DIRECTORY}/dbus/.libs/libdbus-1.so 1>/dev/null 2>&1\; fi\"
  DEPENDS ${DBUS_BUILD_DIRECTORY}/Makefile
  WORKING_DIRECTORY ${DBUS_BUILD_DIRECTORY}
)

install(
  FILES ${DBUS_BUILD_DIRECTORY}/dbus/dbus-arch-deps.h
  DESTINATION ${DBUS_INCLUDE_DIR_ARCH}/dbus
)

install(
  DIRECTORY ${DBUS_SOURCE_DIRECTORY}/dbus/
  DESTINATION ${DBUS_INCLUDE_DIR}/dbus
  FILES_MATCHING PATTERN *.h
)

install(
  DIRECTORY ${DBUS_BUILD_DIRECTORY}/dbus/.libs/
  DESTINATION ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING PATTERN libdbus-1.so*
)
