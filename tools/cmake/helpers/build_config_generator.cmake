function(_add_build_option_record OPTION_NAME BUILD_CONFIG_FILENAME)
  if (NOT ${OPTION_NAME})
    message("Couldn't add option '${OPTION_NAME}' to ${BUILD_CONFIG_FILENAME}. Not specified.")
    return()
  endif()
  get_property(_OPTION_DESCRIPTION CACHE ${OPTION_NAME} PROPERTY HELPSTRING)
  get_property(_OPTION_TYPE CACHE ${OPTION_NAME} PROPERTY TYPE)
  get_property(_OPTION_VALUE CACHE ${OPTION_NAME} PROPERTY VALUE)
  file(APPEND "${BUILD_CONFIG_FILENAME}" "// ${_OPTION_DESCRIPTION}\n")
  file(APPEND "${BUILD_CONFIG_FILENAME}" "${OPTION_NAME}:${_OPTION_TYPE}=${_OPTION_VALUE}\n\n")
endfunction()


# Save build configuration to file BUILD_CONFIG_FILENAME
function(generate_build_config BUILD_CONFIG_FILENAME)
  file(WRITE "${BUILD_CONFIG_FILENAME}" "")

  _add_build_option_record(BUILD_SHARED_LIBS ${BUILD_CONFIG_FILENAME})
  _add_build_option_record(EXTENDED_POLICY ${BUILD_CONFIG_FILENAME})
  _add_build_option_record(BUILD_BT_SUPPORT ${BUILD_CONFIG_FILENAME})
  _add_build_option_record(BUILD_USB_SUPPORT ${BUILD_CONFIG_FILENAME})
  _add_build_option_record(ENABLE_SECURITY ${BUILD_CONFIG_FILENAME})
  # TODO: Uncomment when options will be integrated.
  # _add_build_option_record(EXTENDED_MEDIA_MODE ${BUILD_CONFIG_FILENAME})
  _add_build_option_record(TELEMETRY_MONITOR ${BUILD_CONFIG_FILENAME})
  _add_build_option_record(HMI ${BUILD_CONFIG_FILENAME})
  _add_build_option_record(ENABLE_HMI_PTU_DECRYPTION ${BUILD_CONFIG_FILENAME})
endfunction()
