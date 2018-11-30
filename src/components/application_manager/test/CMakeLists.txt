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

file(COPY smartDeviceLink_test2.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY hmi_capabilities.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY hmi_capabilities_sc1.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY hmi_capabilities_sc2.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libPolicy.so
                   COMMAND ${CMAKE_COMMAND} -E copy ${POLICY_PATH}/libPolicy.so
                           ${CMAKE_CURRENT_BINARY_DIR})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resumption)
file(COPY
     smartDeviceLink_test.ini
     DESTINATION
     ${CMAKE_CURRENT_BINARY_DIR}/resumption)

set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=${CMAKE_CURRENT_BINARY_DIR}")

project(application_manager_test)
create_test(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${COMPONENTS_DIR} ${POLICY_MOCK_INCLUDE_PATH}
                                   ${APPLICATION_MANAGER_DIR}/test/include/)

target_sources(${PROJECT_NAME}
               PRIVATE
               ${APPLICATION_MANAGER_DIR}/test/mock_message_helper.cc
               ${APPLICATION_MANAGER_DIR}/test/mobile_message_handler_test.cc
               ${APPLICATION_MANAGER_DIR}/test/mobile_message_handler_v1_test.cc
               ${APPLICATION_MANAGER_DIR}/test/request_info_test.cc
               ${APPLICATION_MANAGER_DIR}/test/resumption_sql_queries_test.cc
               ${APPLICATION_MANAGER_DIR}/test/event_engine_test.cc
               ${APPLICATION_MANAGER_DIR}/test/policy_event_observer_test.cc
               ${APPLICATION_MANAGER_DIR}/test/application_impl_test.cc
               ${APPLICATION_MANAGER_DIR}/test/hmi_capabilities_test.cc
               ${APPLICATION_MANAGER_DIR}/test/hmi_language_handler_test.cc
               ${APPLICATION_MANAGER_DIR}/test/application_state_test.cc
               ${APPLICATION_MANAGER_DIR}/test/usage_statistics_test.cc
               ${APPLICATION_MANAGER_DIR}/test/policy_handler_test.cc
               ${APPLICATION_MANAGER_DIR}/test/application_manager_impl_test.cc
               ${APPLICATION_MANAGER_DIR}/test/application_helper_test.cc
               ${APPLICATION_MANAGER_DIR}/test/command_holder_test.cc)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE application_manager::application_manager
                              Policy::usage_statistics components::interface)

project(application_manager_mock_hmi_test)
create_test(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${POLICY_MOCK_INCLUDE_PATH}
                                   ${APPLICATION_MANAGER_DIR}/test/include/)

target_sources(
  ${PROJECT_NAME} PRIVATE
  ${APPLICATION_MANAGER_DIR}/test/application_manager_impl_mock_hmi_test.cc)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE application_manager::application_manager
                              components::interface)

project(help_prompt_manager_test)
create_test(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${POLICY_MOCK_INCLUDE_PATH}
                                   ${APPLICATION_MANAGER_DIR}/test/include/)

target_sources(${PROJECT_NAME}
               PRIVATE
               ${APPLICATION_MANAGER_DIR}/test/help_prompt_manager_test.cc
               ${APPLICATION_MANAGER_DIR}/test/mock_message_helper.cc)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE application_manager::application_manager
                              components::interface sdl_rpc_plugins_static_lib)

project(request_controller_test)
create_test(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${POLICY_MOCK_INCLUDE_PATH}
                                   ${APPLICATION_MANAGER_DIR}/test/include/)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  ${APPLICATION_MANAGER_DIR}/test/request_controller/request_controller_test.cc
  ${APPLICATION_MANAGER_DIR}/test/mock_message_helper.cc)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE application_manager::application_manager
                              components::interface)

project(data_resumption_test)
create_test(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${POLICY_MOCK_INCLUDE_PATH}
                                   ${APPLICATION_MANAGER_DIR}/test/include/)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  ${APPLICATION_MANAGER_DIR}/test/resumption/resumption_data_test.cc
  ${APPLICATION_MANAGER_DIR}/test/resumption/resumption_data_db_test.cc
  ${APPLICATION_MANAGER_DIR}/test/resumption/resumption_data_json_test.cc
  ${APPLICATION_MANAGER_DIR}/test/resumption/resume_ctrl_test.cc
  ${APPLICATION_MANAGER_DIR}/test/mock_message_helper.cc)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE application_manager::application_manager
                              components::interface)

add_subdirectory(state_controller)
add_subdirectory(app_launch)
add_subdirectory(commands)
add_subdirectory(message_helper)
