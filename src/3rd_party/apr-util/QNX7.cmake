set(APR_UTIL_CONFIGURE_FLAGS
    "--host=${CMAKE_SYSTEM_PROCESSOR}-nto-qnx"
    "--bindir=${QNX_HOST}/usr/bin/"
    "--prefix=${3RD_PARTY_INSTALL_PREFIX}"
    "--with-apr=../apr-build"
    "--with-expat-source=${EXPAT_SOURCE_DIRECTORY}"
    "--with-expat-build=${EXPAT_BUILD_DIRECTORY}"
    "LDFLAGS=-L${QNX_HOST}/usr/lib"
    "CPPFLAGS=-I${QNX_HOST}/usr/include"
    "MAKE=${QNX_HOST}/usr/bin/make${HOST_EXECUTABLE_SUFFIX}"
    "STRIP=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-strip${HOST_EXECUTABLE_SUFFIX}"
    "OBJDUMP=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-objdump${HOST_EXECUTABLE_SUFFIX}"
    "OBJCOPY=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-objcopy${HOST_EXECUTABLE_SUFFIX}"
    "LINKER=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ld"
    "NM=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-nm${HOST_EXECUTABLE_SUFFIX}"
    "RANLIB=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ranlib${HOST_EXECUTABLE_SUFFIX}"
    "AR=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ar${HOST_EXECUTABLE_SUFFIX}"
    "CXX=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-g++${HOST_EXECUTABLE_SUFFIX}"
    "CC=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-gcc${HOST_EXECUTABLE_SUFFIX}"
)


add_custom_command(OUTPUT ${APR_UTIL_BUILD_DIRECTORY}/Makefile
    COMMAND ${APR_UTIL_SOURCE_DIRECTORY}/configure ${APR_UTIL_CONFIGURE_FLAGS}
    DEPENDS libapr-1
    DEPENDS expat
    WORKING_DIRECTORY ${APR_UTIL_BUILD_DIRECTORY}
)


add_custom_target(apr-util 
  ALL make
  DEPENDS ${APR_UTIL_BUILD_DIRECTORY}/Makefile
  WORKING_DIRECTORY ${APR_UTIL_BUILD_DIRECTORY}
)
