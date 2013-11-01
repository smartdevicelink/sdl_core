include_directories (
  ./include
  ../application_manager/include/
  ../utils/include/
  ../smart_objects/include/
  ../formatters/include/
  ../../thirdPartyLibs/jsoncpp/include/
)

set (SOURCES
    ./src/policy_manager.cc
    ./src/policy_configuration.cc
    ./src/policy_table.cc
    ./src/policy_table_schema.cc
)


add_library("policies" ${SOURCES})
target_link_libraries ("policies" formatters jsoncpp)