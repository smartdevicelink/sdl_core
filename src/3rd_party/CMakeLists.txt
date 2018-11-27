# Copyright (c) 2018, Ford Motor Company All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

include("./set_3rd_party_paths.cmake")

set(3RD_PARTY_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(3RD_PARTY_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if(ENABLE_LOG)
  # --- libexpat
  add_subdirectory(expat-2.1.0)
  set(EXPAT_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)

  set(FORCE_APR_BUILD OFF)
  # --- apr-util
  add_subdirectory(apr-util)

  # --- libapr-1
  add_subdirectory(apr)

  # --- log4cxx
  add_subdirectory(log4cxx)
endif()

add_subdirectory(boost)

# --- jsoncpp
add_subdirectory(./jsoncpp)
set(JSONCPP_INCLUDE_DIRECTORY ${JSONCPP_INCLUDE_DIRECTORY} PARENT_SCOPE)

# --- encryption
add_subdirectory(./encryption)
set(ENCRYPTION_INCLUDE_DIRECTORY ${ENCRYPTION_INCLUDE_DIRECTORY} PARENT_SCOPE)

# --- gmock
if(BUILD_TESTS)
  add_subdirectory(./gmock-1.7.0)
  set(GMOCK_INCLUDE_DIRECTORY ${GMOCK_INCLUDE_DIRECTORY} PARENT_SCOPE)
endif()

include("./install_3rd_party_libs.cmake")     

add_custom_target(3rd_party make
                  WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY})

add_custom_target(install_3rd_party make install
                  WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY})
