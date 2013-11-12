include_directories (
  ./include
  ../application_manager/include/
  ../utils/include/
  ../smart_objects/include/
  ../formatters/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ${CMAKE_BINARY_DIR}/src/components/
)

set (SOURCES
    ./src/policy_manager_impl.cc
    ./src/policy_configuration.cc
    ./src/policy_table.cc
    ./src/policy_table_schema.cc
    ./src/preloaded_pt_schema.cc
    ./src/permissions_calculator.cc
)


add_library("policies" ${SOURCES})
target_link_libraries ("policies" formatters jsoncpp SmartObjects)