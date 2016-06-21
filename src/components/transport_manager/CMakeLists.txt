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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/platform.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

find_package(SDLLibUSB REQUIRED)

include_directories(
  ${COMPONENTS_DIR}/transport_manager/include
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/protocol_handler/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/resumption/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LIBUSB_INCLUDE_DIRECTORY}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(EXCLUDE_PATHS
  ${COMPONENTS_DIR}/transport_manager/include/transport_manager/usb/qnx
  ${COMPONENTS_DIR}/transport_manager/src/usb/qnx
)

set(LIBRARIES
  protocol
)

if(BUILD_BT_SUPPORT)
  get_os(OS)
  if(${OS} STREQUAL "posix")
    list(APPEND LIBRARIES
      bluetooth
    )
  elseif(${OS} STREQUAL "win")
    list(APPEND LIBRARIES
      Bthprops.lib
      ws2_32
    )
  endif()
else()
  list(APPEND EXCLUDE_PATHS
    ${COMPONENTS_DIR}/transport_manager/include/transport_manager/bluetooth
    ${COMPONENTS_DIR}/transport_manager/src/bluetooth
  )
endif()

if(BUILD_USB_SUPPORT)
  list(APPEND LIBRARIES
    ${LIBUSB_LIBRARIES}
  )
else()
  list(APPEND EXCLUDE_PATHS
    ${COMPONENTS_DIR}/transport_manager/include/transport_manager/usb
    ${COMPONENTS_DIR}/transport_manager/src/usb
  )
endif()

collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

add_library(transport_manager ${SOURCES})
target_link_libraries(transport_manager ${LIBRARIES})

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
