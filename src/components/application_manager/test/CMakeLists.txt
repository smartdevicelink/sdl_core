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

# TODO{ALeshin}: APPLINK-10792. Do not write tests which use
# application manager(AM) singleton while refactoring of AM is finished.

if (BUILD_TESTS)

  include_directories(
    ${GMOCK_INCLUDE_DIRECTORY}
    ${CMAKE_BINARY_DIR}/src/components/
    ${COMPONENTS_DIR}/application_manager/include/
    ${COMPONENTS_DIR}/utils/include/
    ${COMPONENTS_DIR}/resumption/include/
    ${COMPONENTS_DIR}/utils/include/
    ${COMPONENTS_DIR}/policy/include/
    ${COMPONENTS_DIR}/media_manager/include/
    ${COMPONENTS_DIR}/security_manager/include/
    ${COMPONENTS_DIR}/policy/test/include/
    ${COMPONENTS_DIR}/application_manager/test/include/
    )

  set(testSources
    ${AM_TEST_DIR}/mobile_message_handler_test.cc
    ${AM_TEST_DIR}/mobile_message_handler_v1_test.cc
    ${AM_TEST_DIR}/request_info_test.cc
    ${AM_TEST_DIR}/resumption_sql_queries_test.cc
    ${AM_TEST_DIR}/event_engine_test.cc
    ${AM_TEST_DIR}/policy_event_observer_test.cc
    ${AM_TEST_DIR}/application_impl_test.cc
    ${AM_TEST_DIR}/hmi_capabilities_test.cc
    ${AM_TEST_DIR}/application_state_test.cc
    ${AM_TEST_DIR}/usage_statistics_test.cc
    ${AM_TEST_DIR}/policy_handler_test.cc
    ${AM_TEST_DIR}/mock_message_helper.cc
  )
  set (request_controller_SOURCES
    ${AM_TEST_DIR}/request_controller/request_controller_test.cc
  )

set(testLibraries
  ApplicationManager
  Utils
  dbms
  jsoncpp
  Policy
  connectionHandler
  HMI_API
  MOBILE_API
  v4_protocol_v1_2_no_extra
  SmartObjects
  formatters
  gmock_main
  UsageStatistics
  dl
  ProtocolLibrary
  ConfigProfile
  MediaManager
  Resumption
  ProtocolHandler
)

if(ENABLE_SECURITY)
  list(APPEND testLibraries SecurityManager)
endif()

list (APPEND testLibraries sqlite3 -L${SQLITE3_LIBRARY_DIR} -Wl,-rpath,/opt/i386/lib)

set(test_exec_libraries
  HMI_API
  MOBILE_API
  v4_protocol_v1_2_no_extra
  SmartObjects
  formatters
  ProtocolHandler
  connectionHandler
  HMIMessageHandler
  Utils
  jsoncpp
  ConfigProfile
  MediaManager
  Resumption
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  list(REMOVE_ITEM test_exec_libraries dl)
endif()

  set(testLibraries
    Utils
    ApplicationManager
    jsoncpp
    Policy
    connectionHandler
    HMI_API
    MOBILE_API
    v4_protocol_v1_2_no_extra
    SmartObjects
    formatters
    gmock_main
    UsageStatistics
    dl
    ProtocolLibrary
    ConfigProfile
    MediaManager
    Resumption
    ProtocolHandler
  )

  if(ENABLE_SECURITY)
    list(APPEND testLibraries SecurityManager)
  endif()
  
  list (APPEND testLibraries sqlite3 -L${SQLITE3_LIBRARY_DIR} -Wl,-rpath,/opt/i386/lib)

  if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
    list(REMOVE_ITEM test_exec_libraries dl)
  endif()

  if (ENABLE_LOG)
    list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
    list(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
    list(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
    list(APPEND LIBRARIES expat -L${EXPAT_LIBS_DIRECTORY})
  endif()

  file(COPY smartDeviceLink_test2.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libPolicy.so
                     COMMAND ${CMAKE_COMMAND} -E
                     copy  ${COMPONENTS_DIR}/policy/libPolicy.so ${CMAKE_CURRENT_BINARY_DIR})
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=${CMAKE_CURRENT_BINARY_DIR}")
  create_test("application_manager_test" "${testSources}" "${testLibraries}" )
# TODO [AKozoriz] : Fix not buildable tests
 set(ResumptionData_SOURCES
    ${AM_TEST_DIR}/resumption/resumption_data_test.cc
    ${AM_TEST_DIR}/resumption/resumption_data_db_test.cc
    ${AM_TEST_DIR}/resumption/resumption_data_json_test.cc
    ${AM_TEST_DIR}/resumption/resume_ctrl_test.cc
    ${AM_TEST_DIR}/mock_message_helper.cc
  )

  file(COPY hmi_capabilities.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resumption)
  file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resumption)
  create_test("resumption/data_resumption_test" "${ResumptionData_SOURCES}" "${testLibraries}")

  add_subdirectory(state_controller)
endif()

add_subdirectory(app_launch)
add_subdirectory(commands)
