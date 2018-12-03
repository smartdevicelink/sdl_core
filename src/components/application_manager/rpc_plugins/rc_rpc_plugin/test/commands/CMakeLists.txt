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

project(rc_commands_test)

create_test(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${APPLICATION_MANAGER_DIR}/test/include
                                   ${APPLICATION_MANAGER_DIR}/include
                                   ${RC_PLUGIN_TEST_DIR}/include
                                   ${COMPONENTS_DIR})

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  "${COMPONENTS_DIR}/application_manager/test/mock_message_helper.cc"
  "${CMAKE_CURRENT_LIST_DIR}/button_press_request_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/get_interior_vehicle_data_request_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/on_interior_vehicle_data_notification_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/on_remote_control_settings_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/rc_get_interior_vehicle_data_consent_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/set_interior_vehicle_data_request_test.cc")

target_link_libraries(${PROJECT_NAME}
                      PRIVATE components::interface rpc_plugins_static_lib)
