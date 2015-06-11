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


set(CH_SRC_DIR ${COMPONENTS_DIR}/connection_handler/src)

include_directories (
    include
    ${COMPONENTS_DIR}/protocol_handler/include/
    ${COMPONENTS_DIR}/config_profile/include/
    ${COMPONENTS_DIR}/utils/include/
    ${ENCRYPTION_INCLUDE_DIRECTORY}
    ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ${CH_SRC_DIR}/connection_handler_impl.cc
    ${CH_SRC_DIR}/connection.cc
    ${CH_SRC_DIR}/device.cc
    ${CH_SRC_DIR}/heartbeat_monitor.cc
)

set(LIBRARIES
    ConfigProfile
    ProtocolLibrary
)

add_library(connectionHandler ${SOURCES})
target_link_libraries(connectionHandler encryption)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()