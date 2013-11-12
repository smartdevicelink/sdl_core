cmake_minimum_required(VERSION 2.8.9)

include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/qt_hmi/qml_model
)

set (LIBRARIES
)

set (SOURCES
   ./src/tst_qt_hmi.cc
)


create_test("qt_hmi" "${SOURCES}" "${LIBRARIES}")
