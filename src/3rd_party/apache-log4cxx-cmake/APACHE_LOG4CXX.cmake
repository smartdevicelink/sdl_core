# Copyright (c) 2018, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

set(LOG4CXX_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../logging-log4cxx_qnx)

set(CONFIGURE_FLAGS
    "--host=${CMAKE_SYSTEM_PROCESSOR}-nto-qnx"
    "--bindir=${QNX_HOST}/usr/bin/"
    "--with-apr=../apr-build"
    "--with-apr-util=../apr-util-build"
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

# set(SIMPLEDATEFORMAT_PATH
#   "${LOG4CXX_SOURCE_DIRECTORY}/src/main/include/log4cxx/helpers/simpledateformat.h")

# set(STRINGHELPER_PATH
#   "${LOG4CXX_SOURCE_DIRECTORY}/src/main/cpp/stringhelper.cpp")

# set(SIMPLEDATEFORMAT_PATCH 
#    sed -Ei "s/namespace std '\{' class locale'\;' '\}'/#include \<locale\>\\nusing std::locale'\;'/g" ${SIMPLEDATEFORMAT_PATH}
# )

# set(STRINGHELPER_PATCH 
#    sed -Ei "s/#include \<apr.h\>/#include \<apr.h\>\\n#include \<stdlib.h\>/g" ${STRINGHELPER_PATH}
# )


add_custom_command(OUTPUT ${LOG4CXX_BUILD_DIRECTORY}/Makefile  
  # COMMAND ${SIMPLEDATEFORMAT_PATCH} 
  # COMMAND ${STRINGHELPER_PATCH} 
  COMMAND ${LOG4CXX_SOURCE_DIRECTORY}/configure ${CONFIGURE_FLAGS}  
  DEPENDS libapr-1
  DEPENDS apr-util
  WORKING_DIRECTORY ${LOG4CXX_BUILD_DIRECTORY}
)

add_custom_target(liblog4cxx ALL make
  DEPENDS ${LOG4CXX_BUILD_DIRECTORY}/Makefile
  WORKING_DIRECTORY ${LOG4CXX_BUILD_DIRECTORY}
)
