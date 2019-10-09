# Copyright (c) 2019, Ford Motor Company
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.

# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.

# Neither the name of the copyright holders nor the names of their contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.

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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/application_manager/test/include
  ${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/include
  ${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/test/include/
  ${COMPONENTS_DIR}/include/test/application_manager/
  ${COMPONENTS_DIR}/rc_rpc_plugin/test/include
)

set (RESOURCE_ALLOC_MANAGER_TEST_SOURCES
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/interior_data_manager_impl.cc
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/interior_data_cache_impl.cc
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/resource_allocation_manager_impl.cc
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/rc_rpc_plugin.cc
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/rc_app_extension.cc
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/rc_consent_manager_impl.cc
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/rc_capabilities_manager_impl.cc
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/rc_command_factory.cc
${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/test/mock_rc_helpers.cc
${CMAKE_CURRENT_SOURCE_DIR}/resource_allocation_manager_impl_test.cc
)

set(RC_COMMANDS_TEST_SOURCE_DIR ${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/test/commands)
set(RC_COMMANDS_SOURCE_DIR ${COMPONENTS_DIR}/application_manager/rpc_plugins/rc_rpc_plugin/src/commands)
collect_sources(COMMANDS_SOURCES "${RC_COMMANDS_SOURCE_DIR}" "${RC_COMMANDS_TEST_SOURCE_DIR}")

set(LIBRARIES
  ApplicationManager
  connectionHandler
  SmartObjects
  ProtocolHandler
  MessageHelper
  connectionHandler
  Utils
  Resumption
  jsoncpp
  gmock_main
  dl
)

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
  list(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
  list(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
  list(APPEND LIBRARIES expat -L${EXPAT_LIBS_DIRECTORY})
endif()

list(APPEND COMMANDS_SOURCES ${RESOURCE_ALLOC_MANAGER_TEST_SOURCES})

create_test("resource_allocation_manager_test" "${COMMANDS_SOURCES}" "${LIBRARIES}")

