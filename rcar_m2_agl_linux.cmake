# Setup Crosscompiling for AGL(Poky toolchain) on Renesas Rcar M2
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR armv7)
SET(CMAKE SYSTEM_VERSION 1)

# Setup the cross compiler
SET(CMAKE_C_COMPILER ${CC})
SET(CMAKE_CXX_COMPILER ${CXX})

ADD_DEFINITIONS(-DBLUEZ5)

SET(CROSS_HOST "arm-agl-linux-gnueabi")
SET(CROSS_APR_VARS 
    "ac_cv_file__dev_zero=yes"
    "ac_cv_func_setpgrp_void=yes"
    "apr_cv_tcp_nodelay_with_cork=yes"
    "apr_cv_process_shared_works=yes"
    "apr_cv_mutex_robust_shared=no"
    "ac_cv_sizeof_struct_iovec=8"
    "ac_cv_sizeof_long=4"
    "apr_cv_mutex_recursive=yes")

# Setup SysRoot
SET(CMAKE_SYSROOT $ENV{SDKTARGETSYSROOT})
SET(CMAKE_FIND_ROOT_PATH $ENV{SDKTARGETSYSROOT})

#Set searching directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
