include_directories (
  ./include
  ../utils/include/
)

set (SOURCES
    ./src/CSmartObject.cpp
    ./src/CSmartSchema.cpp
    ./src/ISchemaItem.cpp
    ./src/CAlwaysFalseSchemaItem.cpp
    ./src/CAlwaysTrueSchemaItem.cpp
    ./src/CArraySchemaItem.cpp
    ./src/CBoolSchemaItem.cpp
    ./src/CObjectSchemaItem.cpp
    ./src/CStringSchemaItem.cpp
    ./src/TEnumSchemaItem.cpp
    ./src/TNumberSchemaItem.cpp
)

add_library("SmartObjects" ${SOURCES})
target_link_libraries ("SmartObjects" log4cxx)
