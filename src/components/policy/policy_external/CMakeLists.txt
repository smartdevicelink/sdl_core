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

# --- Policy
project(Policy)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} SHARED)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(${PROJECT_NAME}_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

target_sources(${PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src/access_remote_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/cache_manager.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/policy_helper.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/policy_manager_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/policy_table.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/sql_pt_ext_queries.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/sql_pt_ext_representation.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/sql_pt_queries.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/sql_pt_representation.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/status.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/update_status_manager.cc"
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    jsoncpp
    utils::utils
    utils::dbms
    config_profile::config_profile
    Policy::policy_table
  PRIVATE
    components::interface
)

if(ENABLE_HMI_PTU_DECRYPTION)
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      USE_HMI_PTU_DECRYPTION
  )
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "${${PROJECT_NAME}_INCLUDE_DIR}"
)

install(TARGETS ${PROJECT_NAME}
  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  COMPONENT sdl_core
  PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

add_subdirectory(src/policy_table)
add_subdirectory(src/usage_statistics)
