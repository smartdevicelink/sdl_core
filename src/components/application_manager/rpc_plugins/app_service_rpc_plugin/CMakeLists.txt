# Copyright (c) 2019, Ford Motor Company, Livio
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
# Neither the name of the copyright holders nor the names of their contributors
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

include_directories(
  include
  ${COMPONENTS_DIR}/application_manager/rpc_plugins/sdl_rpc_plugin/include
  )

set (COMMANDS_SOURCE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/src/commands
)

set (MOBILE_COMMANDS_SOURCE_DIR
  ${COMMANDS_SOURCE_DIR}/mobile
)

set (HMI_COMMANDS_SOURCE_DIR
  ${COMMANDS_SOURCE_DIR}/hmi
)

collect_sources(MOBILE_COMMANDS "${MOBILE_COMMANDS_SOURCE_DIR}")
collect_sources(HMI_COMMANDS "${HMI_COMMANDS_SOURCE_DIR}")
collect_sources(PLUGIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src")
collect_sources(COMMANDS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/commands")

set(LIBRARIES
  HMI_API
  MOBILE_API
  ApplicationManager
  v4_protocol_v1_2_no_extra
  SmartObjects
  Utils
  sdl_rpc_plugin
)

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

add_library("app_service_rpc_plugin" SHARED ${PLUGIN_SOURCES} ${MOBILE_COMMANDS} ${HMI_COMMANDS} ${COMMANDS_SOURCES})
target_link_libraries("app_service_rpc_plugin" ${LIBRARIES})

add_library("AppServiceRpcPluginStaticLib" ${PLUGIN_SOURCES} ${MOBILE_COMMANDS} ${HMI_COMMANDS})
target_link_libraries("AppServiceRpcPluginStaticLib" ${LIBRARIES})

set(INSTALL_DESTINATION bin)

install(TARGETS app_service_rpc_plugin
  DESTINATION ${INSTALL_DESTINATION}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
