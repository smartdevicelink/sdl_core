set(LOG4CXX_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/logging-log4cxx_qnx)

set(CONFIGURE_FLAGS
    "--host=${CMAKE_SYSTEM_PROCESSOR}-nto-qnx"
    "--bindir=${QNX_HOST}/usr/bin/"
    "--with-apr=../../apr/apr-build"
    "--with-apr-util=../../apr-util/apr-util-build"
    "--disable-dot"
	  "--disable-doxygen"
    "--disable-html-docs"
    "ac_cv_file__dev_zero=yes"
    "ac_cv_func_setpgrp_void=yes"
    "apr_cv_process_shared_works=yes"
    "apr_cv_mutex_recursive=yes"
    "apr_cv_mutex_robust_shared=no"
    "apr_cv_tcp_nodelay_with_cork=no"
    "ac_cv_sizeof_struct_iovec=8"   
    "LDFLAGS=-L${EXPAT_LIBS_DIRECTORY} -L${QNX_HOST}/usr/lib"
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

add_custom_command(OUTPUT ${LOG4CXX_BUILD_DIRECTORY}/Makefile  
  COMMAND ${LOG4CXX_SOURCE_DIRECTORY}/configure ${CONFIGURE_FLAGS}  
  DEPENDS libapr-1
  DEPENDS apr-util
  WORKING_DIRECTORY ${LOG4CXX_BUILD_DIRECTORY}
)

add_custom_target(liblog4cxx ALL make
  DEPENDS ${LOG4CXX_BUILD_DIRECTORY}/Makefile
  WORKING_DIRECTORY ${LOG4CXX_BUILD_DIRECTORY}
)

