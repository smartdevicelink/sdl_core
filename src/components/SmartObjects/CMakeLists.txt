include_directories (
  ./include
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/CSmartObject.cpp
    ./src/CSmartSchema.cpp
    ./src/ISchemaItem.cpp
    ./src/CAlwaysTrueSchemaItem.cpp
    ./src/CBoolSchemaItem.cpp
    ./src/TNumberSchemaItem.cpp
)

add_library("SmartObjects" ${SOURCES})