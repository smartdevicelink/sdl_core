include_directories (
  ./include
  ../utils/include/
)

set (SOURCES
    ./src/smart_object.cc
    ./src/smart_schema.cc
    ./src/schema_item.cc
    ./src/always_false_schema_item.cc
    ./src/always_true_schema_item.cc
    ./src/default_shema_item.cc
    ./src/bool_schema_item.cc
    ./src/number_schema_item.cc
    ./src/enum_schema_item.cc
    ./src/string_schema_item.cc
    ./src/object_schema_item.cc
    ./src/array_schema_item.cc
)

add_library("SmartObjects" ${SOURCES})

if(ENABLE_LOG)
  target_link_libraries("SmartObjects" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

