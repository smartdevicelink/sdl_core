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
# Neither the name of the copyright holders nor the names of their contributors
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

project(rpc_plugins)

include(GNUInstallDirs)

add_library(${PROJECT_NAME} MODULE)

set(
  PLUGIN_SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/src/interior_data_cache_impl.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/interior_data_manager_impl.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/rc_app_extension.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/rc_command_factory.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/rc_helpers.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/rc_rpc_plugin.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/resource_allocation_manager_impl.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/rc_command_request.cc"
  # commands/hmi
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_button_press_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_button_press_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_get_interior_vehicle_data_consent_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_get_interior_vehicle_data_consent_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_get_interior_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_get_interior_vehicle_data_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_on_interior_vehicle_data_notification.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_on_remote_control_settings_notification.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_set_interior_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/rc_set_interior_vehicle_data_response.cc"
  # commands/mobile
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/button_press_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/button_press_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/get_interior_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/get_interior_vehicle_data_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/on_interior_vehicle_data_notification.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/set_interior_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/set_interior_vehicle_data_response.cc"
  )

target_sources(${PROJECT_NAME} PRIVATE ${PLUGIN_SOURCES})

target_link_libraries(${PROJECT_NAME}
                      PUBLIC rpc_base::rpc_base
                             utils::utils
                             smart_objects::smart_objects
                             interfaces::HMI_API
                             interfaces::MOBILE_API
                             interfaces::v4_protocol_v1_2_no_extra
                             connection_handler::connection_handler
                      PRIVATE application_manager::interface
                              components::interface)

target_include_directories(${PROJECT_NAME}
                           PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

install(TARGETS ${PROJECT_NAME}
        DESTINATION "${SDL_PLUGINS_INSTALL_DIR}"
        COMPONENT
        sdl_core
        PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE)

if(BUILD_TESTS)
  add_library("rpc_rpc_plugin_static_lib" ${PLUGIN_SOURCES})

  target_link_libraries("rpc_rpc_plugin_static_lib"
                        PUBLIC rpc_base::rpc_base
                               utils::utils
                               smart_objects::smart_objects
                               interfaces::HMI_API
                               interfaces::MOBILE_API
                               interfaces::v4_protocol_v1_2_no_extra
                               connection_handler::connection_handler
                        PRIVATE application_manager::interface
                                components::interface)

  target_include_directories("rpc_rpc_plugin_static_lib"
                             PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
  add_subdirectory(test)
endif()
