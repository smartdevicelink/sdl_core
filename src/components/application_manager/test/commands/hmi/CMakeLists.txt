# Copyright (c) 2016, Ford Motor Company
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

include_directories(
  ${GMOCK_INCLUDE_DIRECTORY}
  ${AM_SOURCE_DIR}/include/application_manager/commands/
  ${AM_TEST_DIR}/include/
  ${AM_TEST_DIR}/include/application_manager/
  ${AM_HMI_COMMANDS_DIR}
)

set (SOURCES
  ${COMPONENTS_DIR}/application_manager/src/message_helper.cc
  ${AM_SOURCE_DIR}/src/resumption/resumption_data_json.cc
  ${AM_SOURCE_DIR}/src/resumption/resume_ctrl.cc
  ${COMPONENTS_DIR}/resumption/src/last_state.cc
  ${AM_SOURCE_DIR}/src/smart_object_keys.cc
  ${AM_TEST_DIR}/commands/hmi/dummy_hmi_commands_test.cc

  ${AM_HMI_COMMANDS_DIR}/on_vi_gps_data_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_speed_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_rpm_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_fuel_level_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_fuel_level_state_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_instant_fuel_consumption_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_external_temperature_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_vin_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_prndl_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_tire_pressure_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_odometer_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_belt_status_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_body_information_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_device_status_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_driver_braking_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_wiper_status_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_head_lamp_status_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_engine_torque_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_acc_pedal_position_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_steering_wheel_angle_notification.cc
  ${AM_HMI_COMMANDS_DIR}/on_vi_my_key_notification.cc
  ${AM_HMI_COMMANDS_DIR}/ui_set_icon_request.cc
)

set(LIBRARIES
  gmock
  Utils
  ApplicationManager
  connectionHandler
  HMI_API
  MOBILE_API
  SmartObjects
  jsoncpp
)

create_test("dummy_hmi_commands_test" "${SOURCES}" "${LIBRARIES}" )