# Copyright (c) 2014, Ford Motor Company
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

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  if (CMAKE_SYSTEM_PROCESSOR MATCHES arm*)
    set (ARCHITECTURE arm) # log4cxx doesn't recognize armv7
  else ()
    set (ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
  endif ()

  add_custom_target(liblog4cxx ALL CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ./configure --prefix=${CMAKE_INSTALL_PREFIX} --host=${ARCHITECTURE}-nto-qnx --with-apr=${CMAKE_INSTALL_PREFIX} --with-apr-util=${CMAKE_INSTALL_PREFIX}
    COMMAND make
    DEPENDS libapr-1
    DEPENDS apr-util
  )
else ()
  add_custom_target(liblog4cxx ./configure --prefix=${CMAKE_INSTALL_PREFIX}
    COMMAND make
    DEPENDS libapr-1
    DEPENDS apr-util
  )
endif ()
