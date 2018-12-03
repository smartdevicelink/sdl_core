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

function(add_rpc_plugin_dynamic NAME)
  include(GNUInstallDirs)
  add_library(${NAME} MODULE)

  target_sources(${NAME} PRIVATE ${PLUGIN_SOURCES})

  target_link_libraries(${NAME}
                        PUBLIC rpc_base::rpc_base
                               utils::utils
                               smart_objects::smart_objects
                               interfaces::HMI_API
                               interfaces::MOBILE_API
                               interfaces::v4_protocol_v1_2_no_extra
                               connection_handler::connection_handler
                               application_manager::interface
                               components::interface)

  target_include_directories(${NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

  install(TARGETS ${NAME}
          DESTINATION "${SDL_PLUGINS_INSTALL_DIR}"
          COMPONENT
          sdl_core
          PERMISSIONS
          OWNER_READ
          OWNER_WRITE
          OWNER_EXECUTE
          GROUP_READ
          GROUP_EXECUTE
          WORLD_READ
          WORLD_EXECUTE)
endfunction()

function(add_rpc_plugin_static NAME)
  add_library("${NAME}_static_lib" ${PLUGIN_SOURCES})

  target_link_libraries("${NAME}_static_lib"
                        PUBLIC rpc_base::rpc_base
                               utils::utils
                               smart_objects::smart_objects
                               interfaces::HMI_API
                               interfaces::MOBILE_API
                               interfaces::v4_protocol_v1_2_no_extra
                               connection_handler::connection_handler
                               application_manager::interface
                               components::interface)

  target_include_directories("${NAME}_static_lib"
                             PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
endfunction()
