# Copyright (c) 2018, Ford Motor Company
# All rights reserved.
#
# Copyright (c) 2018 Xevo Inc.
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
# Neither the name of the copyright holders nor the names of their contributors
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

project(application_manager)

add_library(${PROJECT_NAME}_INTERFACE INTERFACE)
add_library(${PROJECT_NAME}::interface ALIAS ${PROJECT_NAME}_INTERFACE)

target_include_directories(${PROJECT_NAME}_INTERFACE
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    #app_launch
    "${CMAKE_CURRENT_LIST_DIR}/src/app_launch/app_launch_ctrl_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/app_launch/app_launch_data_db.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/app_launch/app_launch_data_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/app_launch/app_launch_data_json.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/app_launch/app_launch_sql_queries.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/app_launch/apps_launcher.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/app_launch/device_apps_launcher.cc"

    #commands
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/command_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/command_notification_from_mobile_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/command_notification_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/command_request_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/command_response_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/notification_from_hmi.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/notification_to_hmi.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/pending.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/request_from_hmi.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/request_to_hmi.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/response_from_hmi.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/commands/response_to_hmi.cc"

    #event_engine
    "${CMAKE_CURRENT_LIST_DIR}/src/event_engine/event.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/event_engine/event_dispatcher_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/event_engine/event_observer.cc"

    #helpers
    "${CMAKE_CURRENT_LIST_DIR}/src/helpers/application_helper.cc"

    #message_helper
    "${CMAKE_CURRENT_LIST_DIR}/src/message_helper/message_helper.cc"

    #plugin_manager
    "${CMAKE_CURRENT_LIST_DIR}/src/plugin_manager/rpc_plugin_manager_impl.cc"

    #policies
    "${CMAKE_CURRENT_LIST_DIR}/src/policies/policy_event_observer.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/policies/policy_handler.cc"
    #"${CMAKE_CURRENT_LIST_DIR}/src/policies/policy_retry_sequence.cc"
    #policy/delegates
    "${CMAKE_CURRENT_LIST_DIR}/src/policies/delegates/app_permission_delegate.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/policies/delegates/statistics_delegate.cc"

    #resumption
    "${CMAKE_CURRENT_LIST_DIR}/src/resumption/resume_ctrl_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/resumption/resumption_data.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/resumption/resumption_data_db.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/resumption/resumption_data_json.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/resumption/resumption_sql_queries.cc"

    #system_time
    "${CMAKE_CURRENT_LIST_DIR}/src/system_time/system_time_handler_impl.cc"

    #application_manager
    "${CMAKE_CURRENT_LIST_DIR}/src/application_data_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/application_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/application_manager_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/application_state.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/command_holder_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/hmi_capabilities_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/hmi_interfaces_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/hmi_language_handler.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/hmi_state.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/message.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/mobile_message_handler.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/request_controller.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/request_info.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/request_tracker.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/rpc_handler_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/rpc_service_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/smart_object_keys.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/state_controller_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/usage_statistics.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/help_prompt_manager_impl"
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    utils::utils
    smart_objects::smart_objects
    resumption::resumption
    interfaces::HMI_API
    interfaces::MOBILE_API
    interfaces::v4_protocol_v1_2_no_extra
    connection_handler::connection_handler
    hmi_message_handler::hmi_message_handler
    Policy::Policy
    Policy::usage_statistics
    bson
  PRIVATE
    components::interface
)

if(TELEMETRY_MONITOR)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      telemetry_monitor::telemetry_monitor
  )
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

add_subdirectory(rpc_plugins)
