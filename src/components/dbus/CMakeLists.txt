include_directories (
  ./include/
  ${CMAKE_SOURCE_DIR}/
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_SOURCE_DIR}/src/components/formatters/include/
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  /usr/include/dbus-1.0/
  /usr/lib/i386-linux-gnu/dbus-1.0/include/
)

set (SOURCES
  ./src/schema.cc
  ${CMAKE_CURRENT_BINARY_DIR}/message_descriptions.cc
  ${CMAKE_CURRENT_BINARY_DIR}/introspection_xml.cc
  ./src/dbus_message_controller.cc
  ./src/dbus_adapter.cc
)

set (LIBRARIES
    dbus-1
)

add_library("DBus" ${SOURCES})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/message_descriptions.cc
  COMMAND python codegen/make_message_descriptions.py --infile ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/HMI_API.xml
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/HMI_API.xml codegen/make_message_descriptions.py
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/introspection.xml
  COMMAND python codegen/make_introspection.py --infile ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/HMI_API.xml
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/HMI_API.xml codegen/make_introspection.py
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/introspection_xml.cc
  COMMAND python codegen/make_introspection_c.py --infile ${CMAKE_CURRENT_BINARY_DIR}/introspection.xml
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/introspection.xml
)

target_link_libraries("DBus" "${LIBRARIES}")
