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

cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_AUTOMOC ON)

set(components_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(dbus_dir ${components_dir}/dbus)
set(codegen_dir ${dbus_dir}/codegen)
set(interfaces_dir ${components_dir}/interfaces)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/qml_dbus.cc ${CMAKE_CURRENT_SOURCE_DIR}/qml_dbus.h
  COMMAND python ${codegen_dir}/make_qml_dbus_cpp.py --infile ${interfaces_dir}/HMI_API.xml
  --outdir ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCES
  dbus_plugin.cc
  qml_dbus.cc
  sdlproxy.cc
  hmiproxy.cc
  stream_qvariant.cc
  moc_qml_dbus.cpp
)

qt5_generate_moc(qml_dbus.h moc_qml_dbus.cpp)

set(target DbusAdapter)

add_library(${target} SHARED ${SOURCES})

qt5_use_modules(${target} Core DBus Qml Quick)

set(model_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../qml_model)
set(destination ${model_dir}/com/ford/sdl/hmi/dbus_adapter)

target_link_libraries(${target} log4cxx)

install(TARGETS ${target}
  DESTINATION ${destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
install(FILES qmldir DESTINATION ${destination})

add_custom_command(TARGET ${target} POST_BUILD COMMAND make install)
add_custom_command(TARGET ${target} POST_BUILD COMMAND ${qmlplugindump_binary} -notrelocatable com.ford.sdl.hmi.dbus_adapter 1.0 ${model_dir} > ${destination}/plugins.qmltypes)