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

if(GENERATORS_INCLUDED)
  return()
endif()
set(GENERATORS_INCLUDED ON)

find_package(PythonInterp REQUIRED)

get_filename_component(INTEFRACE_GENERATOR_DIR
  "../../InterfaceGenerator"
  REALPATH
  BASE_DIR "${CMAKE_CURRENT_LIST_DIR}")
file(GLOB_RECURSE INTERFACE_GENERATOR_DEPENDENCIES "${INTEFRACE_GENERATOR_DIR}/*")

function(generate_interface XML_INPUT ARG_NAMESPACE PARSER_TYPE TARGET_NAME)
  set(GENERATOR_OUTPUT_DIR "${CMAKE_BINARY_DIR}/InterfaceGenerator/output")
  set(GENERATE_OUTPUT_TARGET "${GENERATOR_OUTPUT_DIR}/${TARGET_NAME}/interfaces")

  get_filename_component(FILE_NAME ${XML_INPUT} NAME_WE)
  if("${FILE_NAME}" STREQUAL "QT_HMI_API")
    set(GENERATED_SOURCES
      "${GENERATE_OUTPUT_TARGET}/HMI_API.h"
      "${GENERATE_OUTPUT_TARGET}/HMI_API_schema.h"
      "${GENERATE_OUTPUT_TARGET}/HMI_API_schema.cc")

    add_custom_command(
      OUTPUT ${GENERATED_SOURCES}
      COMMAND
        "${PYTHON_EXECUTABLE}"
          -B "${INTEFRACE_GENERATOR_DIR}/Generator.py"
          "${XML_INPUT}"
          "${ARG_NAMESPACE}"
          "${GENERATE_OUTPUT_TARGET}"
          "--parser-type" "${PARSER_TYPE}"
      COMMAND
        ${CMAKE_COMMAND}
          -DINPUT_FILE="${GENERATE_OUTPUT_TARGET}/QT_HMI_API.h"
          -DMATCH_TEXT="QT_HMI_API"
          -DREPLACE_WITH="HMI_API"
          -DOUTPUT_FILE="${GENERATE_OUTPUT_TARGET}/HMI_API.h"
          -P "${CMAKE_SOURCE_DIR}/tools/cmake/helpers/string_replace.cmake"
      COMMAND
        ${CMAKE_COMMAND}
          -DINPUT_FILE="${GENERATE_OUTPUT_TARGET}/QT_HMI_API_schema.h"
          -DMATCH_TEXT="QT_HMI_API"
          -DREPLACE_WITH="HMI_API"
          -DOUTPUT_FILE="${GENERATE_OUTPUT_TARGET}/HMI_API_schema.h"
          -P "${CMAKE_SOURCE_DIR}/tools/cmake/helpers/string_replace.cmake"
      COMMAND
        ${CMAKE_COMMAND}
          -DINPUT_FILE="${GENERATE_OUTPUT_TARGET}/QT_HMI_API_schema.cc"
          -DMATCH_TEXT="QT_HMI_API"
          -DREPLACE_WITH="HMI_API"
          -DOUTPUT_FILE="${GENERATE_OUTPUT_TARGET}/HMI_API_schema.cc"
          -P "${CMAKE_SOURCE_DIR}/tools/cmake/helpers/string_replace.cmake"
      DEPENDS "${INTERFACE_GENERATOR_DEPENDENCIES}" "${XML_INPUT}"
      COMMENT "Generating files:\n   ${GENERATED_SOURCES}\nfrom:\n   ${XML_INPUT} ...")
  else()
    set(GENERATED_SOURCES
      "${GENERATE_OUTPUT_TARGET}/${FILE_NAME}.h"
      "${GENERATE_OUTPUT_TARGET}/${FILE_NAME}_schema.h"
      "${GENERATE_OUTPUT_TARGET}/${FILE_NAME}_schema.cc")

    add_custom_command(
      OUTPUT ${GENERATED_SOURCES}
      COMMAND
        "${PYTHON_EXECUTABLE}"
          -B "${INTEFRACE_GENERATOR_DIR}/Generator.py"
          "${XML_INPUT}"
          "${ARG_NAMESPACE}"
          "${GENERATE_OUTPUT_TARGET}"
          "--parser-type" "${PARSER_TYPE}"
      DEPENDS "${INTERFACE_GENERATOR_DEPENDENCIES}" "${XML_INPUT}"
      COMMENT "Generating files:\n   ${GENERATED_SOURCES}\nfrom:\n   ${XML_INPUT} ...")
  endif()

  target_sources(${TARGET_NAME}
    PRIVATE
      ${GENERATED_SOURCES}
  )
  target_include_directories(${TARGET_NAME}
    PUBLIC "${GENERATOR_OUTPUT_DIR}/${TARGET_NAME}"
  )
endfunction()
