if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  add_custom_target(libapr-1 ALL CC=$ENV{QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-gcc RANLIB=$ENV{QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ranlib AS=$ENV{QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-as AR=$ENV{QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ar ./configure --prefix=`readlink -f ${CMAKE_CURRENT_SOURCE_DIR}/../../..`/3rd_party --host=${CMAKE_SYSTEM_PROCESSOR}-nto-qnx ac_cv_file__dev_zero=yes ac_cv_func_setpgrp_void=yes apr_cv_process_shared_works=yes apr_cv_mutex_recursive=yes apr_cv_mutex_robust_shared=no apr_cv_tcp_nodelay_with_cork=no ac_cv_sizeof_struct_iovec=8
    COMMAND make
    COMMAND make install
  )
else ()
  add_custom_target(libapr-1 ALL ./configure --prefix=`readlink -f ${CMAKE_CURRENT_SOURCE_DIR}/../../..`/3rd_party
    COMMAND make
    COMMAND make install
  )
endif ()
