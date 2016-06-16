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
        include
        ${CMAKE_BINARY_DIR}/src/components/
        ${COMPONENTS_DIR}/resumption/include
        ${GMOCK_INCLUDE_DIRECTORY}
        ${COMPONENTS_DIR}/application_manager/include/
        ${COMPONENTS_DIR}/application_manager/include/application_manager/commands/
        ${COMPONENTS_DIR}/utils/include/utils
        ${COMPONENTS_DIR}/include/utils
        ${COMPONENTS_DIR}/include/
        ${COMPONENTS_DIR}/policy/include/policy
        ${COMPONENTS_DIR}/policy/test/include
        ${COMPONENTS_DIR}/media_manager/include/
        ${SecurityManagerIncludeDir}
        ${COMPONENTS_DIR}/security_manager/include
        ${AM_TEST_DIR}/resumption/include/
        ${AM_SOURCE_DIR}/policy/src/policy/policy_table/table_struct
    )

    set(testSources
        ${AM_TEST_DIR}/application_state_test.cc
        ${AM_TEST_DIR}/application_impl_test.cc
        ${AM_TEST_DIR}/event_engine_test.cc
        ${AM_TEST_DIR}/hmi_capabilities_test.cc
        ${AM_TEST_DIR}/mobile_message_handler_test.cc
        ${AM_TEST_DIR}/mobile_message_handler_v1_test.cc
        ${AM_TEST_DIR}/mock_message_helper.cc
        ${AM_TEST_DIR}/policy_event_observer_test.cc
        ${AM_TEST_DIR}/request_info_test.cc
        ${AM_TEST_DIR}/resumption_sql_queries_test.cc
        ${AM_TEST_DIR}/usage_statistics_test.cc
        ${AM_TEST_DIR}/policy_handler_test.cc
      )

    set (request_controller_SOURCES
        ${AM_TEST_DIR}/request_controller/request_controller_test.cc
    )

    set(AM_SOURCES
        ${AM_SOURCE_DIR}/src/policies/policy_handler.cc
        ${AM_SOURCE_DIR}/src/policies/policy_event_observer.cc
        ${AM_SOURCE_DIR}/src/policies/delegates/app_permission_delegate.cc
        ${AM_SOURCE_DIR}/src/policies/delegates/statistics_delegate.cc

        ${AM_SOURCE_DIR}/src/event_engine/event_dispatcher_impl.cc
        ${AM_SOURCE_DIR}/src/event_engine/event_observer.cc
        ${AM_SOURCE_DIR}/src/event_engine/event.cc

        ${COMMANDS_SOURCE_DIR}/command_impl.cc
        ${COMMANDS_SOURCE_DIR}/command_request_impl.cc
        ${COMMANDS_SOURCE_DIR}/command_response_impl.cc
        ${COMMANDS_SOURCE_DIR}/command_notification_impl.cc
        ${COMMANDS_SOURCE_DIR}/command_notification_from_mobile_impl.cc
        ${COMMANDS_SOURCE_DIR}/pending.cc

        ${AM_SOURCE_DIR}/src/resumption/resume_ctrl.cc
        ${AM_SOURCE_DIR}/src/resumption/resumption_data_db.cc
        ${AM_SOURCE_DIR}/src/resumption/resumption_data_json.cc
        ${AM_SOURCE_DIR}/src/resumption/resumption_data.cc
        ${AM_SOURCE_DIR}/src/resumption/resumption_sql_queries.cc

        ${AM_SOURCE_DIR}/src/usage_statistics.cc
        ${AM_SOURCE_DIR}/src/request_info.cc
        ${AM_SOURCE_DIR}/src/message.cc
        ${AM_SOURCE_DIR}/src/application_impl.cc
        ${AM_SOURCE_DIR}/src/application_state.cc
        ${AM_SOURCE_DIR}/src/state_controller.cc
        ${AM_SOURCE_DIR}/src/mobile_command_factory.cc
        ${AM_TEST_DIR}/mock_message_helper.cc

        ${AM_SOURCE_DIR}/src/hmi_command_factory.cc
        ${AM_SOURCE_DIR}/src/hmi_state.cc

        ${AM_SOURCE_DIR}/src/hmi_capabilities.cc
        ${AM_SOURCE_DIR}/src/hmi_language_handler.cc
        ${AM_SOURCE_DIR}/src/application_data_impl.cc
        ${AM_SOURCE_DIR}/src/request_controller.cc

        ${COMMANDS_SOURCE_DIR}/mobile/register_app_interface_request.cc
        ${COMMANDS_SOURCE_DIR}/mobile/unregister_app_interface_request.cc

        ${AM_SOURCE_DIR}/src/mobile_message_handler.cc
        ${COMMANDS_SOURCE_DIR}/mobile/put_file_request.cc
        ${COMMANDS_SOURCE_DIR}/command_request_impl.cc
    )

    set(testLibraries
      dbms
      Utils
      ApplicationManager
      jsoncpp
      Policy
      connectionHandler
      HMI_API
      MOBILE_API
      v4_protocol_v1_2_no_extra
      smart_object
      formatters
      gmock_main
      UsageStatistics
      dl
      ProtocolLibrary
      ConfigProfile
      MediaManager
      Resumption
      ProtocolHandler
      SecurityManager
    )

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      list(REMOVE_ITEM testLibraries dl)
    endif()

    if(ENABLE_LOG)
      list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
      list(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
      list(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
      list(APPEND LIBRARIES expat -L${EXPAT_LIBS_DIRECTORY})
    endif()

    file(COPY smartDeviceLink_test2.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      link_directories ($ENV{SDL_SQLITE_DIR})
    endif()

    if(QT_PORT)
      link_directories (${CMAKE_SOURCE_DIR}/build/openssl_win_x86/lib)
    endif()

    if(NOT QT_PORT AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      link_directories(${CMAKE_SOURCE_DIR}/build/openssl_win_x64/lib/)
    endif()

    add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libPolicy.so
                         COMMAND ${CMAKE_COMMAND} -E
                         copy  ${COMPONENTS_DIR}/policy/libPolicy.so ${CMAKE_CURRENT_BINARY_DIR}
         )

    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    create_test("application_manager_test" "${testSources}" "${testLibraries}")

    # TODO [AKozoriz] : Fix not buildable tests
    set(ResumptionData_SOURCES
    #    ${AM_TEST_DIR}/resumption/resumption_data_test.cc
    #    ${AM_TEST_DIR}/resumption/resumption_data_db_test.cc
    #    ${AM_TEST_DIR}/resumption/resumption_data_json_test.cc
    #    ${AM_TEST_DIR}/resumption/resume_ctrl_test.cc
        ${AM_TEST_DIR}/mock_message_helper.cc
      )

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj" )
    endif()

    file(COPY hmi_capabilities.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resumption)
    file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resumption)
    create_test("data_resumption_test" "${ResumptionData_SOURCES}" "${testLibraries}")

    add_subdirectory(state_controller)
    add_subdirectory(commands)
