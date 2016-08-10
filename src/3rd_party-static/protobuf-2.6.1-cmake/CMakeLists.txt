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

set(GPB_SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../protobuf-2.6.1")
set(GPB_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../protobuf-2.6.1-build")
set(PROTOC_BUILD_PATH /tmp/protobuf-2.6.1)
set(PROTOC_PATH /tmp/protobuf-2.6.1/src/protoc)

set(GPB_INCLUDE_DIRECTORY ${GPB_SOURCE_DIRECTORY}/src)
set(GPB_INCLUDE_DIRECTORY ${GPB_INCLUDE_DIRECTORY} PARENT_SCOPE)
set(GPB_LIBS_DIRECTORY ${GPB_BUILD_DIRECTORY}/src/.libs)
set(GPB_LIBS_DIRECTORY ${GPB_LIBS_DIRECTORY} PARENT_SCOPE)

file(MAKE_DIRECTORY
  ${GPB_BUILD_DIRECTORY}
)

set(COMMON_CONFIGURE_FLAGS
  "--with-protoc=${PROTOC_PATH}"
  "CXXFLAGS=-D_SINGLE_THREADE"
)

if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
  set(CONFIGURE_FLAGS "--host=${CMAKE_SYSTEM_PROCESSOR}-nto-qnx" "--build=${CMAKE_SYSTEM_PROCESSOR}-nto-qnx" ${COMMON_CONFIGURE_FLAGS})
else()
  set(CONFIGURE_FLAGS ${COMMON_CONFIGURE_FLAGS})
endif()

add_custom_command(
    OUTPUT  protoc
    COMMAND "PROJECT_ROOT=${CMAKE_SOURCE_DIR}" "BINARY_DIR=${CMAKE_BINARY_DIR}" "PROTOC_BUILD_PATH=${PROTOC_BUILD_PATH}" "PATH_TO_PROTOBUF_REPO=${GPB_SOURCE_DIRECTORY}" "./build_protoc.sh"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
    COMMENT "Build protoc using HOST environment"
)

add_custom_command(OUTPUT ${GPB_BUILD_DIRECTORY}/Makefile
  DEPENDS protoc
  COMMAND CXX=${CMAKE_CXX_COMPILER} ${GPB_SOURCE_DIRECTORY}/configure ${CONFIGURE_FLAGS}
  WORKING_DIRECTORY ${GPB_BUILD_DIRECTORY}
  COMMENT "Build protobuff using ${PROTOC_PATH}"
)

add_custom_target(LibGPB ALL make
  DEPENDS ${GPB_BUILD_DIRECTORY}/Makefile
  WORKING_DIRECTORY ${GPB_BUILD_DIRECTORY}
)

install(
  DIRECTORY ${GPB_BUILD_DIRECTORY}/src/.libs/
  DESTINATION ${CMAKE_BINARY_DIR}/bin/lib
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING PATTERN libprotobuf.so*
)

macro(generate_proto PROTO_FILE LIB_NAME)
  set(PROTOC_PATH /tmp/protobuf-2.6.1/src/protoc)
  get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
  get_filename_component(PROTO_DIRECTORY ${PROTO_FILE} DIRECTORY)
  STRING(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" GENERATE_PATH ${PROTO_DIRECTORY})
  add_definitions(-DGOOGLE_PROTOBUF_NO_RTTI)
  MESSAGE(STATUS "Folder of generated files : ${GENERATE_PATH}")
  MESSAGE(STATUS "File name: ${PROTO_FILE}")
  MESSAGE(STATUS "File folder: ${PROTO_DIRECTORY}")
  MESSAGE(STATUS "Lib name : ${LIB_NAME}")
  ADD_CUSTOM_COMMAND(
    OUTPUT ${GENERATE_PATH}/${PROTO_NAME}.pb.h ${GENERATE_PATH}/${PROTO_NAME}.pb.cc
    COMMAND mkdir -p ${GENERATE_PATH}
    COMMAND
      BUILD_PATH=${CMAKE_BINARY_DIR}
      ${PROTOC_PATH}
      --proto_path=${PROTO_DIRECTORY}
      --cpp_out=${GENERATE_PATH}
      ${PROTO_FILE}
    VERBATIM
    DEPENDS ${PROTO_FILE} LibGPB
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating ${GENERATE_PATH}/${PROTO_NAME}.pb.h ${GENERATE_PATH}/${PROTO_NAME}.pb.cc")
  set_source_files_properties(${GENERATE_PATH}/${PROTO_NAME}.pb.cc
                              PROPERTIES GENERATED TRUE)
  INCLUDE_DIRECTORIES(${GENERATE_PATH})
  INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src/components)

  IF (NOT TARGET ${LIB_NAME})
    ADD_LIBRARY(
      ${LIB_NAME}
      ${GENERATE_PATH}/${PROTO_NAME}.pb.cc
    )
    set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "-Wno-sign-compare")
  endif(NOT TARGET ${LIB_NAME})
endmacro(generate_proto)

