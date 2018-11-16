set(CONFIGURE_FLAGS
  "--with-apr=../../apr/apr-build"
  "--with-expat-source=${EXPAT_SOURCE_DIRECTORY}"
  "--with-expat-build=${EXPAT_BUILD_DIRECTORY}"
)

add_custom_command(OUTPUT ${APR_UTIL_BUILD_DIRECTORY}/Makefile
  COMMAND CC=${CMAKE_C_COMPILER} ${APR_UTIL_SOURCE_DIRECTORY}/configure ${CONFIGURE_FLAGS}
  DEPENDS libapr-1
  DEPENDS expat
  WORKING_DIRECTORY ${APR_UTIL_BUILD_DIRECTORY}
  )


add_custom_target(apr-util ALL make
  DEPENDS ${APR_UTIL_BUILD_DIRECTORY}/Makefile
  WORKING_DIRECTORY ${APR_UTIL_BUILD_DIRECTORY}
)
