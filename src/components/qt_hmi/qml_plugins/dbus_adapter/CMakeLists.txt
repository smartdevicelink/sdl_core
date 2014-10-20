# Copyright (c) 2013, Ford Motor Company
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

cmake_minimum_required(VERSION 2.8.11)

set(components_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(dbus_dir ${components_dir}/dbus)
set(codegen_dir ${dbus_dir}/codegen)
set(codegen ${codegen_dir}/make_qml_dbus_cpp.py)
set(codegen_request_to_sdl ${codegen_dir}/make_request_to_sdl.py)
set(codegen_hmi_requests ${codegen_dir}/make_hmi_requests.py)
set(codegen_notification_qml ${codegen_dir}/make_notifications_qml.py)
set(parser ${codegen_dir}/ford_xml_parser.py)
set(interfaces_dir ${components_dir}/interfaces)
set(api_xml ${interfaces_dir}/QT_HMI_API.xml)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qml_dbus.cc ${CMAKE_CURRENT_BINARY_DIR}/qml_dbus.h
  COMMAND python ${codegen} --infile ${api_xml} --version ${qt_version} --outdir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${api_xml} ${codegen} ${parser}
  COMMENT "Generating files:
    ${CMAKE_CURRENT_BINARY_DIR}/qml_dbus.cc
    ${CMAKE_CURRENT_BINARY_DIR}/qml_dbus.h
    from:
    ${api_xml} ..."
)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/request_to_sdl.cc ${CMAKE_CURRENT_BINARY_DIR}/request_to_sdl.h
  COMMAND python ${codegen_request_to_sdl} --infile ${api_xml} --version ${qt_version} --outdir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${api_xml} ${codegen_request_to_sdl} ${parser}
  COMMENT "Generating files:
    ${CMAKE_CURRENT_BINARY_DIR}/request_to_sdl.cc
    ${CMAKE_CURRENT_BINARY_DIR}/request_to_sdl.h
    from:
    ${api_xml} ..."
)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/hmi_requests.cc ${CMAKE_CURRENT_BINARY_DIR}/hmi_requests.h
  COMMAND python ${codegen_hmi_requests} --infile ${api_xml} --version ${qt_version} --outdir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${api_xml} ${codegen_hmi_requests} ${parser}
  COMMENT "Generating files:
    ${CMAKE_CURRENT_BINARY_DIR}/hmi_requests.cc
    ${CMAKE_CURRENT_BINARY_DIR}/hmi_requests.h
    from:
    ${api_xml} ..."
)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sdl_proxy.cc ${CMAKE_CURRENT_BINARY_DIR}/sdl_proxy.h
  COMMAND python ${codegen_notification_qml} --infile ${api_xml} --version ${qt_version} --outdir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${api_xml} ${codegen_notification_qml} ${parser}
  COMMENT "Generating files:
    ${CMAKE_CURRENT_BINARY_DIR}/sdl_proxy.cc
    ${CMAKE_CURRENT_BINARY_DIR}/sdl_proxy.h
    from:
    ${api_xml} ..."
)

set(target DbusAdapter)
set(destination com/ford/sdl/hmi/dbus_adapter)
set(install_destination bin/hmi/plugins/${destination})
set(library_name ${CMAKE_SHARED_LIBRARY_PREFIX}${target}${CMAKE_SHARED_LIBRARY_SUFFIX})

set(CMAKE_AUTOMOC ON)
set(SOURCES
  dbus_plugin.cc
  ${CMAKE_CURRENT_BINARY_DIR}/qml_dbus.cc
  moc_qml_dbus.cpp
  sdl_proxy.cc
  moc_sdl_proxy.cpp
  hmi_proxy.cc
  stream_qvariant.cc
  moc_qml_dbus.cpp
  dbus_controller.cc
  request_to_sdl.cc
  moc_request_to_sdl.cpp
  hmi_requests.cc
  moc_hmi_requests.cpp
)
include_directories(
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/src/components
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
)

add_library(${target} SHARED ${SOURCES})

if (${qt_version} MATCHES "4.8.5")
  qt4_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/hmi_requests.h moc_hmi_requests.cpp)
  qt4_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/request_to_sdl.h moc_request_to_sdl.cpp)
  qt4_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/qml_dbus.h moc_qml_dbus.cpp)
  qt4_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/sdl_proxy.h moc_sdl_proxy.cpp)
  qt4_use_modules(${target} Core DBus Declarative)
  set(model_dir ${CMAKE_CURRENT_BINARY_DIR}/../../qml_model_qt4/)
  set(copy_destination ${CMAKE_CURRENT_BINARY_DIR}/../../qml_model_qt4/${destination})
elseif (${qt_version} MATCHES "5.1.0")
  qt5_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/hmi_requests.h moc_hmi_requests.cpp)
  qt5_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/request_to_sdl.h moc_request_to_sdl.cpp)
  qt5_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/qml_dbus.h moc_qml_dbus.cpp)
  qt5_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/sdl_proxy.h moc_sdl_proxy.cpp)
  qt5_use_modules(${target} Core DBus Qml)
  set(model_dir ${CMAKE_CURRENT_BINARY_DIR}/../../qml_model_qt5/)
  set(copy_destination ${CMAKE_CURRENT_BINARY_DIR}/../../qml_model_qt5/${destination})
endif ()

target_link_libraries(${target} log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
target_link_libraries(${target} Utils)

add_custom_target(copy_library_${target} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/${library_name}
    ${copy_destination}
    DEPENDS ${target}
    COMMENT "Copying library ${library_name}")
file(COPY qmldir DESTINATION ${copy_destination})

install(TARGETS ${target}
  DESTINATION ${install_destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
install(FILES qmldir DESTINATION ${install_destination})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_custom_target(qmltypes_${target} ALL
    COMMAND ${qmlplugindump_binary} -nonrelocatable com.ford.sdl.hmi.dbus_adapter 1.0 ${model_dir} > ${CMAKE_CURRENT_BINARY_DIR}/plugins.qmltypes 2>/dev/null || true
    DEPENDS copy_library_${target}
  )
  add_custom_target(copy_qmltypes_${target} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_BINARY_DIR}/plugins.qmltypes
      ${copy_destination}
    DEPENDS qmltypes_${target}
  )
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plugins.qmltypes DESTINATION ${install_destination})
endif ()
