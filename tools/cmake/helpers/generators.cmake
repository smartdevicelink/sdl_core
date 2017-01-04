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

find_package(PythonInterp REQUIRED)

if(NOT PYTHONINTERP_FOUND)
  message(STATUS "Python interpreter is not found")
  message(STATUS "To install it type in the command line:")
  message(STATUS "sudo apt-get install python")
  message(FATAL_ERROR "Exiting!")
endif(NOT PYTHONINTERP_FOUND)

set(INTEFRACE_GENERATOR "${CMAKE_SOURCE_DIR}/tools/InterfaceGenerator/Generator.py")
set(INTEFRACE_GENERATOR_CMD ${PYTHON_EXECUTABLE} -B ${INTEFRACE_GENERATOR})
file(GLOB_RECURSE INTERFACE_GENERATOR_DEPENDENCIES "${CMAKE_SOURCE_DIR}/tools/InterfaceGenerator/*.*")

macro(generate_interface ARG_XML_NAME ARG_NAMESPACE PARSER_TYPE)
  string(REGEX MATCH "^[a-zA-Z_0-9]*[^.]" FILE_NAME ${ARG_XML_NAME})  # TODO: make expression more robust

  set(HPP_FILE
    "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.h"
    "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}_schema.h"
  )

  set(CPP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}_schema.cc")
  set(FULL_XML_NAME "${CMAKE_CURRENT_SOURCE_DIR}/${ARG_XML_NAME}")

  add_custom_command(
    OUTPUT ${HPP_FILE} ${CPP_FILE}
    COMMAND ${INTEFRACE_GENERATOR_CMD} ${FULL_XML_NAME} ${ARG_NAMESPACE} ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "${PARSER_TYPE}"
    DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${FULL_XML_NAME}
    COMMENT "Generating files:\n   ${HPP_FILE}\n   ${CPP_FILE}\nfrom:\n   ${FULL_XML_NAME} ..."
    VERBATIM
  )
endmacro()
