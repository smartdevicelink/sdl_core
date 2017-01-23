set(target "CANCooperation")
set(install_destination ${CMAKE_BINARY_DIR}/bin/plugins)
if (ENABLE_GCOV)
  set(GCOV_FLAGS "-ftest-coverage -fprofile-arcs")
else()
  set(GCOV_FLAGS "")
endif()

set(CMAKE_CXX_FLAGS "-fPIC -std=gnu++0x -Wall -Wno-deprecated-declarations -Werror ${GCOV_FLAGS}")

if (CMAKE_BUILD_TYPE)
  if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "")
  else ()
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb3 -DDEBUG")
  endif()
endif()

include_directories (
  ${COMPONENTS_DIR}/include/
  ${COMPONENTS_DIR}/application_manager/include
  ${COMPONENTS_DIR}/can_cooperation/include/
  ${COMPONENTS_DIR}/connection_handler/include/
  ${COMPONENTS_DIR}/functional_module/include/
  ${COMPONENTS_DIR}/config_profile/include/

  ${COMPONENTS_DIR}/hmi_message_handler/include/
  ${COMPONENTS_DIR}/formatters/include/
  ${POLICY_PATH}/include/
  ${POLICY_GLOBAL_INCLUDE_PATH}/
  ${COMPONENTS_DIR}/rpc_base/include/
  ${COMPONENTS_DIR}/application_manager/test/include/

  ${COMPONENTS_DIR}/smart_objects/include/
  ${COMPONENTS_DIR}/utils/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
)

set (CAN_SOURCE_DIR ${COMPONENTS_DIR}/can_cooperation/src)
set (CAN_COMMANDS_DIR ${CAN_SOURCE_DIR}/commands)
set (CAN_VALIDATORS_DIR ${CAN_SOURCE_DIR}/validators)
set (CAN_STRUCT_VALIDATORS_DIR ${CAN_VALIDATORS_DIR}/struct_validators)

set (SOURCES
    ${CAN_SOURCE_DIR}/can_module.cc
    ${CAN_SOURCE_DIR}/can_app_extension.cc
    ${CAN_SOURCE_DIR}/can_tcp_connection.cc
    ${CAN_SOURCE_DIR}/can_module_event.cc
    ${CAN_SOURCE_DIR}/mobile_command_factory.cc
    ${CAN_SOURCE_DIR}/request_controller.cc
    ${CAN_SOURCE_DIR}/message_helper.cc
    ${CAN_SOURCE_DIR}/policy_helper.cc
    ${CAN_SOURCE_DIR}/module_helper.cc
    ${CAN_SOURCE_DIR}/vehicle_capabilities.cc
    ${CAN_COMMANDS_DIR}/base_command_request.cc
    ${CAN_COMMANDS_DIR}/base_command_notification.cc
    ${CAN_COMMANDS_DIR}/button_press_request.cc
    ${CAN_COMMANDS_DIR}/get_interior_vehicle_data_capabilities_request.cc
    ${CAN_COMMANDS_DIR}/get_interior_vehicle_data_request.cc
    ${CAN_COMMANDS_DIR}/set_interior_vehicle_data_request.cc
    ${CAN_COMMANDS_DIR}/on_interior_vehicle_data_notification.cc
    ${CAN_VALIDATORS_DIR}/validator.cc
    ${CAN_VALIDATORS_DIR}/button_press_request_validator.cc
    ${CAN_VALIDATORS_DIR}/get_interior_vehicle_data_capabilities_request_validator.cc
    ${CAN_VALIDATORS_DIR}/get_interior_vehicle_data_request_validator.cc
    ${CAN_VALIDATORS_DIR}/get_interior_vehicle_data_response_validator.cc
    ${CAN_VALIDATORS_DIR}/set_interior_vehicle_data_request_validator.cc
    ${CAN_VALIDATORS_DIR}/on_interior_vehicle_data_notification_validator.cc
    ${CAN_STRUCT_VALIDATORS_DIR}/interior_zone_validator.cc
    ${CAN_STRUCT_VALIDATORS_DIR}/module_description_validator.cc
    ${CAN_STRUCT_VALIDATORS_DIR}/module_data_validator.cc
    ${CAN_STRUCT_VALIDATORS_DIR}/rds_data_validator.cc
    ${CAN_STRUCT_VALIDATORS_DIR}/climate_control_data_validator.cc
    ${CAN_STRUCT_VALIDATORS_DIR}/radio_control_data_validator.cc
)

set (LIBRARIES
    jsoncpp
    FunctionalModule
    Utils
    ConfigProfile
)

add_library(${target} SHARED ${SOURCES})
target_link_libraries(${target} ${LIBRARIES} )

if(ENABLE_LOG)
  target_link_libraries(${target} log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()


install(TARGETS ${target}
  DESTINATION ${install_destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

set(FILES_FOR_COPY
${CMAKE_CURRENT_SOURCE_DIR}/InteriorVehicleDataCapabilities.json
)

install(
  FILES ${FILES_FOR_COPY}
  DESTINATION ${install_destination}
)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
