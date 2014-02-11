set(INTERGEN_CMD ${CMAKE_BINARY_DIR}/tools/intergen/bin/intergen)
set(GENERATED_LIB_DEPENDENCIES rpc_base jsoncpp)
set(GENERATED_LIB_HEADER_DEPENDENCIES
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
)

# Generates interface library from given xml file and builds it
# in CMAKE_CURRENT_BINARY_DIR
# |xml_file_name| contains path to xml spec files
# |generated_interface_names| should contain list of generated interfaces
# from xml_file (intergen creates separate directory for every interface).
# Their names are written lowercase_underscored_style.
macro (GenerateInterfaceLibrary xml_file_name generated_interface_names)
  foreach(interface_name ${generated_interface_names})
    set(HEADERS
        ${interface_name}/enums.h
        ${interface_name}/types.h
        ${interface_name}/functions.h
    )
    set(SOURCES
        ${interface_name}/enums.cc
        ${interface_name}/types.cc
        ${interface_name}/functions.cc
        ${interface_name}/validation.cc
    )
    add_custom_command( OUTPUT ${HEADERS} ${SOURCES}
                        COMMAND ${INTERGEN_CMD} -f ${CMAKE_CURRENT_SOURCE_DIR}/${xml_file_name} -i ${interface_name}
                        DEPENDS ${INTERGEN_CMD} ${xml_file_name}
                        COMMENT "Generating interface ${interface_name} from ${xml_file_name}"
                        VERBATIM
                       )
    include_directories(
      ${CMAKE_CURRENT_BINARY_DIR}
      ${GENERATED_LIB_HEADER_DEPENDENCIES}
    )
    add_library(${interface_name} ${SOURCES})
    target_link_libraries(${interface_name} ${GENERATED_LIB_DEPENDENCIES})
  endforeach(interface_name)
endmacro(GenerateInterfaceLibrary xml_file_name generated_interface_names)
