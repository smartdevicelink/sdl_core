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

INCLUDE("./set_3rd_party_paths.cmake")

if(ENABLE_LOG OR HMI_DBUS_API)
  # --- libexpat
  add_subdirectory(expat-2.1.0)
  set(EXPAT_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)
endif()

if(ENABLE_LOG)
  set(APR_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)
  set(APR_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include PARENT_SCOPE)
  set(APR_UTIL_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)
  set(LOG4CXX_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include PARENT_SCOPE)
  set(LOG4CXX_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)
endif()

if(ENABLE_LOG AND NOT (${is_logger_actual} EQUAL 0))
  # --- libapr-1
  add_subdirectory(apr-cmake)

  # --- apr-util
  add_subdirectory(apr-util-cmake)

  # --- log4cxx
  add_subdirectory(apache-log4cxx-cmake)
endif()

# --- D-Bus
if(HMI_DBUS_API)
  set(DBUS_INCLUDE_DIR ${3RD_PARTY_INSTALL_PREFIX}/include)
  set(DBUS_INCLUDE_DIR_ARCH ${3RD_PARTY_INSTALL_PREFIX_ARCH}/include)
  set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIR} ${DBUS_INCLUDE_DIR_ARCH})
  set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIRS} PARENT_SCOPE)
  set(DBUS_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)
endif()

if(HMI_DBUS_API AND NOT (${is_dbus_actual} EQUAL 0))
  add_subdirectory(dbus-cmake)
endif()

