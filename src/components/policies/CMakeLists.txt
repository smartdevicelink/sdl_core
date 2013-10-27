include_directories (
  ./include
  ../application_manager/include/
  ../utils/include/
  ../smart_objects/include/
)

set (SOURCES
    ./src/policy_manager.cc
    ./src/policy_configuration.cc
    ./src/policy_table.cc
)

add_library("policies" ${SOURCES})