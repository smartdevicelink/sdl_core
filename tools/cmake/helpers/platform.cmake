# Copyright (c) 2016, Ford Motor Company
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

# SDL build platform includes three necessary components:
# OS, Architecture and SDK. Build parameters and sources code base
# could be varied depends on these three components combination.
#
# Platform helpers provide functionality to get supported by SDL
# OS, Architecture or SDK lists and specified for actual build these
# variables values. Values could be passed to cmake as parameters
# (f.e. "-DOS_WINDOWS=1 -DSDK_QT=1"), otherwise they will be detected
# related to current build environment.
#
# Full list of supported cmake flags:
# OS_POSIX
# OS_WINDOWS
# ARCH_X86
# ARCH_X64
# SDK_QT

function(get_supported_os OS_LIST)
  set(${OS_LIST} "posix" "win" PARENT_SCOPE)
endfunction()

function(get_supported_arch ARCH_LIST)
  set(${ARCH_LIST} "x86" "x64" PARENT_SCOPE)
endfunction()

function(get_supported_sdk SDK_LIST)
  set(${SDK_LIST} "qt" PARENT_SCOPE)
endfunction()

function(get_os OS)
  if(OS_POSIX)
    set(${OS} "posix" PARENT_SCOPE)
  elseif(OS_WINDOWS)
    set(${OS} "win" PARENT_SCOPE)
  else()
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      set(${OS} "posix" PARENT_SCOPE)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      set(${OS} "win" PARENT_SCOPE)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "QNX")
      set(${OS} "qnxnto" PARENT_SCOPE)
    else()
      message(FATAL_ERROR "Unsupported operation system")
    endif()
  endif()
endfunction()

function(get_arch ARCH)
  if( CMAKE_SIZEOF_VOID_P MATCHES 8 )
    # void ptr = 8 byte --> x86_64
    set(${ARCH} "x64" PARENT_SCOPE) 
  elseif( CMAKE_SIZEOF_VOID_P MATCHES 4 )
    # void ptr = 4 byte --> x86
    set(${ARCH} "x86" PARENT_SCOPE)
  else()
      message(FATAL_ERROR "Unsupported architecture")
  endif()    
endfunction(get_arch ARCH)

function(get_sdk SDK)
  if(SDK_QT)
    set(${SDK} "qt" PARENT_SCOPE)
  endif()
endfunction()
