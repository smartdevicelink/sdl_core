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

if(BUILD_TESTS)

set(appMain_DIR ${CMAKE_SOURCE_DIR}/src/appMain)

include_directories(
    ${GMOCK_INCLUDE_DIRECTORY}
    ${AM_TEST_DIR}/include/
)

set(LIBRARIES
  gmock
  dbms
  ApplicationManager
  connectionHandler
  Utils
  jsoncpp
)

set(SOURCES
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_button_event_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_navi_tbt_client_state_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_navi_way_point_change_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_ui_command_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_ui_keyboard_input_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_ui_touch_event_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_acc_pedal_position_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_belt_status_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_body_information_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_device_status_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_driver_braking_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_engine_torque_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_external_temperature_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_fuel_level_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_fuel_level_state_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_gps_data_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_head_lamp_status_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_instant_fuel_consumption_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_my_key_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_odometer_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_prndl_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_rpm_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_speed_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_steering_wheel_angle_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_tire_pressure_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_vehicle_data_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_vin_notification.cc
    ${COMPONENTS_DIR}/application_manager/src/commands/hmi/on_vi_wiper_status_notification.cc
    ${AM_TEST_DIR}/mock_message_helper.cc
    hmi_notifications_test.cc
)

create_test("hmi_notifications_test" "${SOURCES}" "${LIBRARIES}")

endif()
