# Copyright (c) 2013, Ford Motor Company
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

# --- Interfaces
add_subdirectory(./interfaces)

# --- TransportManager
add_subdirectory(./transport_manager)

# --- AppMgr
#add_subdirectory(./AppMgr)

# --- formatters
add_subdirectory(./formatters)

# --- ProtocolHandler
add_subdirectory(./protocol_handler)

# --- ConnectionHandler
add_subdirectory(./connection_handler)

# --- Utils
add_subdirectory(./utils)

# --- SmartObjects
add_subdirectory(./smart_objects)

# --- Mobile message handler
add_subdirectory(./mobile_message_handler)

# --- Application manager
add_subdirectory(./application_manager)

# --- Request watchdog
add_subdirectory(./request_watchdog)

# --- HMI Message Handler
add_subdirectory(./hmi_message_handler)

# --- Config
add_subdirectory(./config_profile)

# --- AudioManager
add_subdirectory(./audio_manager)

IF (${QT_HMI})
# --- DBus
    add_subdirectory(./dbus)
# --- Qt HMI
    add_subdirectory(./qt_hmi)
ENDIF (${QT_HMI})
