set(target "CANCooperation")
set(install_destination ${CMAKE_BINARY_DIR}/bin/plugins)
if (ENABLE_GCOV)
  set(GCOV_FLAGS "-ftest-coverage -fprofile-arcs")
else()
  set(GCOV_FLAGS "")
endif()

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

collect_sources(CAN_SOURCES "${CAN_SOURCE_DIR}")
collect_sources(CAN_COMMANDS_SOURCES "${CAN_COMMANDS_DIR}")

set (SOURCES
    ${CAN_SOURCES}
    ${CAN_COMMANDS_SOURCES}
)

set (LIBRARIES
    jsoncpp
    FunctionalModule
    Utils
    ConfigProfile
)
message (${SOURCES})
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
