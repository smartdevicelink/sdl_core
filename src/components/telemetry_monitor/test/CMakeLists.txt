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

if(BUILD_TESTS)

include_directories(
  include
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/telemetry_monitor/include
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/protocol_handler/include
  ${COMPONENTS_DIR}/application_manager/include
  ${COMPONENTS_DIR}/resumption/include
  ${JSONCPP_INCLUDE_DIRECTORY}
)
  

set(testSources
  metric_wrapper_test.cc
  telemetry_monitor_test.cc
  protocol_handler_metric_test.cc
  protocol_handler_observer_test.cc
  transport_manager_metric_test.cc
  transport_manager_observer_test.cc
  application_manager_metric_test.cc
  application_manager_observer_test.cc
)

set(testLibraries
  gmock
  TelemetryMonitor
  jsoncpp
  Utils
  MediaManager
  ProtocolHandler
  v4_protocol_v1_2_no_extra
  SmartObjects
  ProtocolLibrary
  ConfigProfile
  ApplicationManager
  MessageHelper
  Resumption
  jsoncpp
  transport_manager
  MediaManager
  ProtocolHandler
  connectionHandler
)

if (BUILD_USB_SUPPORT)
  list(APPEND testLibraries Libusb-1.0.16)
endif()

if (BUILD_BT_SUPPORT)
  list(APPEND testLibraries bluetooth)
endif()

file(COPY log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

create_test("telemetry_monitor_test" "${testSources}" "${testLibraries}")

endif()
