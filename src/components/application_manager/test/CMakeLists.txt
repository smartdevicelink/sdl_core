# Copyright (c) 2014, Ford Motor Company
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

# Replace include for mocking singltone
get_property(the_include_dirs DIRECTORY "" PROPERTY INCLUDE_DIRECTORIES)
set(class_to_mock ${CMAKE_SOURCE_DIR}/src/components/application_manager/include)
list(FIND the_include_dirs ${class_to_mock} find_idx)
if(find_idx GREATER -1)
  LIST_REPLACE(the_include_dirs ${find_idx} ${AM_MOCK_DIR}/include)
endif()
set_property(DIRECTORY "" PROPERTY INCLUDE_DIRECTORIES ${the_include_dirs})

include_directories(
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/gtest/include
  ${COMPONENTS_DIR}/application_manager/include/application_manager/policies
)

set(testSources
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/src/gmock_main.cc
  ${AM_TEST_DIR}/command_impl_test.cc  
  ${COMPONENTS_DIR}/application_manager/test/mobile_message_handler_test.cc
  ${AM_TEST_DIR}/request_info_test.cc
)

set(mockedSources
  ${AM_MOCK_DIR}/src/application_manager_impl.cc
)

set(AM_SOURCES
  ${AM_SOURCE_DIR}/src/policies/policy_handler.cc
  ${AM_SOURCE_DIR}/src/policies/policy_event_observer.cc

  ${AM_SOURCE_DIR}/src/commands/command_impl.cc
  ${AM_SOURCE_DIR}/src/commands/command_request_impl.cc
  ${AM_SOURCE_DIR}/src/commands/command_response_impl.cc
  ${AM_SOURCE_DIR}/src/commands/command_notification_impl.cc
  ${AM_SOURCE_DIR}/src/commands/command_notification_from_mobile_impl.cc
  ${AM_SOURCE_DIR}/src/commands/pending.cc

  ${AM_SOURCE_DIR}/src/usage_statistics.cc
  ${AM_SOURCE_DIR}/src/request_info.cc
  ${AM_SOURCE_DIR}/src/message.cc
  ${AM_SOURCE_DIR}/src/application_impl.cc
  ${AM_SOURCE_DIR}/src/mobile_command_factory.cc
  ${AM_SOURCE_DIR}/src/message_helper.cc
  ${AM_SOURCE_DIR}/src/hmi_command_factory.cc
  ${AM_SOURCE_DIR}/src/hmi_capabilities.cc
  ${AM_SOURCE_DIR}/src/application_data_impl.cc
  ${AM_SOURCE_DIR}/src/request_controller.cc
  ${AM_SOURCE_DIR}/src/resume_ctrl.cpp
  ${AM_SOURCE_DIR}/src/mobile_message_handler.cc
)

  include_directories(
    ${AM_SOURCE_DIR}/policy/src/policy/policy_table/table_struct
  )

set(testLibraries
  gmock
  gmock_main
  UsageStatistics
  dl
  ProtocolLibrary
)

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

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
  list(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
  list(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
  list(APPEND LIBRARIES expat -L${EXPAT_LIBS_DIRECTORY})
endif()

add_library("ApplicationManagerTest" ${mockedSources} ${testSources} ${AM_SOURCES})

target_link_libraries("ApplicationManagerTest" ${testLibraries} AMHMICommandsLibrary
                                                                AMMobileCommandsLibrary
                                                                AMEventEngine
                                                                AMPolicyLibrary)

create_test("application_manager_test" "${testSources}" "${ApplicationManagerTest}")
target_link_libraries("application_manager_test" ApplicationManagerTest ${test_exec_libraries})

#add_executable(application_manager_test ${testSources})
#target_link_libraries(application_manager_test ApplicationManagerTest ${test_exec_libraries})
