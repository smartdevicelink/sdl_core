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

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  add_custom_command(OUTPUT ${DBUS_SOURCE_DIRECTORY}/Makefile
    COMMAND CC=${CMAKE_C_COMPILER} ./configure --host=${CMAKE_SYSTEM_PROCESSOR}-nto-qnx --enable-tests=no
    WORKING_DIRECTORY ${DBUS_SOURCE_DIRECTORY}
  )
else ()
  add_custom_command(OUTPUT ${DBUS_SOURCE_DIRECTORY}/Makefile
    COMMAND CC=${CMAKE_C_COMPILER} ./configure --enable-tests=no
    WORKING_DIRECTORY ${DBUS_SOURCE_DIRECTORY}
  )
endif ()

add_custom_target(dbus ALL make
  DEPENDS ${DBUS_SOURCE_DIRECTORY}/Makefile
  WORKING_DIRECTORY ${DBUS_SOURCE_DIRECTORY}
)

install(
  PROGRAMS ${DBUS_SOURCE_DIRECTORY}/bus/dbus-daemon
  DESTINATION ${3RD_PARTY_INSTALL_PREFIX}/bin
)

install(
  FILES ${DBUS_SOURCE_DIRECTORY}/bus/session.conf
  DESTINATION ${3RD_PARTY_INSTALL_PREFIX}/etc/dbus-1
)

install(
  DIRECTORY ${DBUS_SOURCE_DIRECTORY}/dbus/.libs/
  DESTINATION ${3RD_PARTY_INSTALL_PREFIX}/lib
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING PATTERN libdbus-1.so*
)
