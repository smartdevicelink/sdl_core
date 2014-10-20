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

set(3RD_PARTY_INSTALL_PREFIX "$ENV{THIRD_PARTY_INSTALL_PREFIX}")
set(USE_DEFAULT_3RD_PARTY_PATH "true")
if(3RD_PARTY_INSTALL_PREFIX)
  set(USE_DEFAULT_3RD_PARTY_PATH "false")
  set(3RD_PARTY_INSTALL_PREFIX_ARCH "$ENV{THIRD_PARTY_INSTALL_PREFIX_ARCH}")
  if(3RD_PARTY_INSTALL_PREFIX_ARCH)
  else()
    set(3RD_PARTY_INSTALL_PREFIX_ARCH ${3RD_PARTY_INSTALL_PREFIX})
  endif()
else()
  if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    set(3RD_PARTY_INSTALL_PREFIX "$ENV{QNX_TARGET}/usr")  
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7")
      set(QNX_ARCH "armle-v7")
    else() 
      set(QNX_ARCH ${CMAKE_SYSTEM_PROCESSOR})
    endif()
    set(3RD_PARTY_INSTALL_PREFIX_ARCH "$ENV{QNX_TARGET}/${QNX_ARCH}/usr")
  else()
    set(3RD_PARTY_INSTALL_PREFIX "/usr/local")
    set(3RD_PARTY_INSTALL_PREFIX_ARCH ${3RD_PARTY_INSTALL_PREFIX})
  endif()
endif()

