set(INTERGEN_CMD ${CMAKE_BINARY_DIR}/tools/intergen/bin/intergen)
set(GENERATED_LIB_DEPENDENCIES jsoncpp)
set(GENERATED_LIB_HEADER_DEPENDENCIES
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
)

# Generates interface library from given xml file and builds it
# in CMAKE_CURRENT_BINARY_DIR
# |xml_file_name| contains path to xml spec files
# |generated_interface_names| should contain list of generated interfaces
# if |AUTO_FUNC_IDS| is added to argument list, intergen is called with "-a"
#   flag telling intergen to generate function ids automatically
# if |DBUS_SUPPORT| is added to argument list, intergen is called with "-d"
#   flag that enables DBus serialization code generation
# from xml_file (intergen creates separate directory for every interface).
# Their names are written lowercase_underscored_style.
function (GenerateInterfaceLibrary xml_file_name generated_interface_names)
  set(options AUTO_FUNC_IDS DBUS_SUPPORT)
  cmake_parse_arguments(GenerateInterfaceLibrary "${options}" "" "" ${ARGN})
  if (GenerateInterfaceLibrary_AUTO_FUNC_IDS)
    set(AUTOID "-a")
  endif()
  if (GenerateInterfaceLibrary_DBUS_SUPPORT)
    set(NEED_DBUS "-d")
    list(APPEND GENERATED_LIB_HEADER_DEPENDENCIES ${DBUS_INCLUDE_DIRS})
  endif()

  foreach(interface_name ${generated_interface_names})
    set(HEADERS
        ${interface_name}/enums.h
        ${interface_name}/types.h
        ${interface_name}/functions.h
        ${interface_name}/interface.h
    )
    set(SOURCES
        ${interface_name}/enums.cc
        ${interface_name}/types.cc
        ${interface_name}/functions.cc
        ${interface_name}/validation.cc
        ${interface_name}/interface.cc
    )
    add_custom_command( OUTPUT ${HEADERS} ${SOURCES}
                        COMMAND ${INTERGEN_CMD} -f ${CMAKE_CURRENT_SOURCE_DIR}/${xml_file_name} -j ${AUTOID} ${NEED_DBUS} -i ${interface_name}
                        DEPENDS ${INTERGEN_CMD} ${xml_file_name}
                        COMMENT "Generating interface ${interface_name} from ${xml_file_name}"
                        VERBATIM
                       )
    include_directories(
      ${CMAKE_CURRENT_BINARY_DIR}
      ${GENERATED_LIB_HEADER_DEPENDENCIES}
    )
    add_library(${interface_name} ${SOURCES})
    add_dependencies(${interface_name} intergen)

    # |previous_interface| ensures that interface libraries are built in the same order
    # as they defined in |generated_interface_names|
    target_link_libraries(${interface_name} ${GENERATED_LIB_DEPENDENCIES} ${previous_interface})
    set(previous_interface ${interface_name})
  endforeach(interface_name)
endfunction(GenerateInterfaceLibrary xml_file_name generated_interface_names)
