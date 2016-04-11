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

if(BUILD_TESTS)

include_directories(
    ${GMOCK_INCLUDE_DIRECTORY}
    ${COMPONENTS_DIR}/media_manager/include
    ${COMPONENTS_DIR}/protocol_handler/include
    ${COMPONENTS_DIR}/application_manager/test/include
    ${COMPONENTS_DIR}/resumption/include

)

if(EXTENDED_MEDIA_MODE)
  include_directories (
    ${GSTREAMER_gst_INCLUDE_DIR}
    ${GLIB_glib_2_INCLUDE_DIR}
  )
endif()

set(SOURCES
    ${COMPONENTS_DIR}/media_manager/test/media_manager_impl_test.cc
)

set(LIBRARIES
    SmartObjects
    MediaManager
    ApplicationManagerTest
    MessageHelper
    ProtocolHandler
    gmock
    connectionHandler
    encryption
    Resumption
    Policy
    ${SecurityManagerLibrary}
)

if(EXTENDED_MEDIA_MODE)
  list(APPEND LIBRARIES
    ${GSTREAMER_gstreamer_LIBRARY})
endif()

create_test("media_manager_test" "${SOURCES}" "${LIBRARIES}")

if(ENABLE_LOG)
    target_link_libraries("media_manager_test" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

endif()

# vim: set ts=2 sw=2 et:
