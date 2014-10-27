#Replace include for mocking singltone
get_property(the_include_dirs DIRECTORY "" PROPERTY INCLUDE_DIRECTORIES)
set(class_to_mock ${CMAKE_SOURCE_DIR}/src/components/application_manager/include)
list(FIND the_include_dirs ${class_to_mock} find_idx)
if(find_idx GREATER -1)
  LIST_REPLACE(the_include_dirs ${find_idx} ${AM_MOCK_DIR}/include)
endif()
set_property(DIRECTORY "" PROPERTY INCLUDE_DIRECTORIES ${the_include_dirs})

include_directories (
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/gtest/include
)

set(testSources
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/src/gmock_main.cc
  ${AM_TEST_DIR}/command_impl_test.cc
)
set (mockedSources
  ${AM_MOCK_DIR}/src/application_manager_impl.cc
)

set (AM_SOURCES
  ${AM_SOURCE_DIR}/src/policies/policy_handler.cc
  ${AM_SOURCE_DIR}/src/policies/policy_event_observer.cc

  ${AM_SOURCE_DIR}/src/commands/command_impl.cc
  ${AM_SOURCE_DIR}/src/commands/command_request_impl.cc
  ${AM_SOURCE_DIR}/src/commands/command_response_impl.cc
  ${AM_SOURCE_DIR}/src/commands/command_notification_impl.cc
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
  list(APPEND AM_SOURCES
    ${COMPONENTS_DIR}/application_manager/src/policies/pt_exchange_handler_impl.cc
    ${COMPONENTS_DIR}/application_manager/src/policies/policy_retry_sequence.cc
  )

set(testLibraries
  gmock
  gtest
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

add_executable(application_manager_test ${testSources})
target_link_libraries(application_manager_test ApplicationManagerTest ${test_exec_libraries})
