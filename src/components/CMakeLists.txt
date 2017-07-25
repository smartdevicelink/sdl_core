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

set(COMPONENTS_DIR ${COMPONENTS_DIR} PARENT_SCOPE)

# --- HMI, MOBILE Interfaces
add_subdirectory(./interfaces)

# --- Protocol Handler
add_subdirectory(./protocol)

# --- Transport Manager
add_subdirectory(./transport_manager)

# --- Resumption
add_subdirectory(./resumption)

# --- Formatters
add_subdirectory(./formatters)

# --- Protocol Handler
add_subdirectory(./protocol_handler)

# --- Connection Handler
add_subdirectory(./connection_handler)

# --- Utils
add_subdirectory(./utils)

# --- Security Manager
if(ENABLE_SECURITY)
  add_subdirectory(./security_manager)
endif()

# --- Policy
if (${EXTENDED_POLICY} STREQUAL "EXTERNAL_PROPRIETARY")
  add_subdirectory(./policy/policy_external/)
  message(STATUS "Use external policy")
else()
  add_subdirectory(./policy/policy_regular/)
  message(STATUS "Use regular policy")
endif()

# --- Validated Types
add_subdirectory(./rpc_base)

# --- Smart Objects
add_subdirectory(./smart_objects)

# --- Application Manager
add_subdirectory(./application_manager)

# --- HMI Message Handler
add_subdirectory(./hmi_message_handler)

# --- Config Profile
add_subdirectory(./config_profile)

# --- Media Manager
add_subdirectory(./media_manager)

if(REMOTE_CONTROL)
  # --- CAN Module
  add_subdirectory(./remote_control)
  # --- Functional module
  add_subdirectory(./functional_module)
endif()
# --- Telemetry Monitor
if(TELEMETRY_MONITOR)
  add_subdirectory(./telemetry_monitor)
endif()

# --- DBus
if(HMI_DBUS_API)
  add_subdirectory(./dbus)
endif()
