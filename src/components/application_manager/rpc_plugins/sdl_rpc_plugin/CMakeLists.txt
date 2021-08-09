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

include_directories(include)

set (COMMANDS_SOURCE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/src/commands
)

set (MOBILE_COMMANDS_SOURCE_DIR
  ${COMMANDS_SOURCE_DIR}/mobile
)

set (HMI_COMMANDS_SOURCE_DIR
  ${COMMANDS_SOURCE_DIR}/hmi
)

set (EXTENSIONS_SOURCE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions
)
collect_sources(MOBILE_COMMANDS "${MOBILE_COMMANDS_SOURCE_DIR}")
collect_sources(HMI_COMMANDS "${HMI_COMMANDS_SOURCE_DIR}")
collect_sources(APP_EXTENSIONS "${EXTENSIONS_SOURCE_DIR}")
collect_sources(RPC_PLUGIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(LIBRARIES
  ApplicationManager
  HMI_API
  MOBILE_API
  connectionHandler
  v4_protocol_v1_2_no_extra
  SmartObjects
  Utils
  PolicyStatic
  jsoncpp
  crypto
)

if(ENABLE_LOG AND NOT ANDROID)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

if (ANDROID)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_library("sdl_rpc_plugin_static" ${MOBILE_COMMANDS} ${HMI_COMMANDS} ${RPC_PLUGIN_SOURCES} ${APP_EXTENSIONS})
target_link_libraries("sdl_rpc_plugin_static" ${LIBRARIES})

add_library("sdl_rpc_plugin" SHARED "src/sdl_rpc_plugin.cc")
target_link_libraries("sdl_rpc_plugin" sdl_rpc_plugin_static)
target_link_libraries("sdl_rpc_plugin" MessageHelper)

if (NOT ANDROID)
  set(INSTALL_DESTINATION bin)

  install(TARGETS sdl_rpc_plugin
    DESTINATION ${INSTALL_DESTINATION}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
  )
else()
  set(LIBRARY_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}sdl_rpc_plugin${CMAKE_SHARED_LIBRARY_SUFFIX})
  add_custom_target(copy_sdl_rpc_plugin ALL
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}
          ${CMAKE_BINARY_DIR}/src/appMain/${LIBRARY_NAME}
          DEPENDS sdl_rpc_plugin
          COMMENT "Copying library ${LIBRARY_NAME}"
          )
endif()

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
