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

set(BOOST_INSTALL_COMMAND ./b2 install)
if(${INSTALL_PATH} MATCHES "/usr/local")
  set(BOOST_INSTALL_COMMAND sudo ./b2 install)
endif()

include(ExternalProject)
externalproject_add(
  Boost
  URL
  http://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
  DOWNLOAD_DIR
  ${BOOST_LIB_SOURCE_DIRECTORY}
  SOURCE_DIR
  ${BOOST_LIB_SOURCE_DIRECTORY}
  CONFIGURE_COMMAND
  ./bootstrap.sh
  --with-libraries=system,thread,date_time,filesystem
  --prefix=${INSTALL_PATH}
  BUILD_COMMAND
  ./b2
  INSTALL_COMMAND
  ${BOOST_INSTALL_COMMAND}
  --with-system
  --with-thread
  --with-date_time
  --with-filesystem
  --prefix=${INSTALL_PATH}
  >
  boost_install.log
  INSTALL_DIR
  ${INSTALL_PATH}
  BUILD_IN_SOURCE
  true)
