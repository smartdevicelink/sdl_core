cmake_minimum_required(VERSION 2.8)

set (PROJECT smartDeviceLinkCore)
project (${PROJECT})

# Please do not change compiler/linker flags if You do not know how particular
# flag is handled by CMake
set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
set(CMAKE_CXX_FLAGS "-g3 -ggdb3 -std=gnu++0x -Werror=return-type -Wuninitialized")
set(CMAKE_CXX_FLAGS_RELEASE "-s") #It will be appended to CMAKE_CXX_FLAGS in release

option( BUILD_SHARED_LIBS "Build all libraries as shared (if ON) or static (if OFF)" OFF)
set(BUILD_TESTS ON)

add_definitions(-DOS_POSIX)
add_definitions(-DOS_LINUX)


# --- Interface generator

find_package(PythonInterp)
if(NOT PYTHONINTERP_FOUND)
    message(ERROR "Python interpreter is not found")
    message(ERROR "To install it type in the command line:")
    message(ERROR "sudo apt-get install python")
endif(NOT PYTHONINTERP_FOUND)

set(INTEFRACE_GENERATOR "${PROJECT_SOURCE_DIR}/tools/InterfaceGenerator/Generator.py")
set(INTEFRACE_GENERATOR_CMD ${PYTHON_EXECUTABLE} -B ${INTEFRACE_GENERATOR})
file(GLOB_RECURSE INTERFACE_GENERATOR_DEPENDENCIES "${PROJECT_SOURCE_DIR}/tools/InterfaceGenerator/*.*")

macro (GenerateInterface arg_xml_name arg_namespace parser_type)
    string(REGEX MATCH "^[a-zA-Z_0-9]*[^.]" file_name ${arg_xml_name})     # TODO: make expression more robust

    set(hpp_file
      "${CMAKE_CURRENT_BINARY_DIR}/${file_name}.h"
      "${CMAKE_CURRENT_BINARY_DIR}/${file_name}_schema.h"
    )

    set(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${file_name}_schema.cc")
    set(full_xml_name "${CMAKE_CURRENT_SOURCE_DIR}/${arg_xml_name}")

    add_custom_command( OUTPUT ${hpp_file} ${cpp_file}
                        COMMAND ${INTEFRACE_GENERATOR_CMD} ${full_xml_name} ${arg_namespace} ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "${parser_type}"
                        DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${full_xml_name}
                        COMMENT "Generating files:\n   ${hpp_file}\n   ${cpp_file}\nfrom:\n   ${arg_xml_name} ..."
                        VERBATIM
                       )

    include_directories (
        ../../../src/components/SmartObjects/include
        ../../../src/components/formatters/include/
        ${CMAKE_BINARY_DIR}
    )

    add_library("${file_name}" ${cpp_file})
endmacro(GenerateInterface)

# --- Useful macro
macro(create_test NAME SOURCES LIBS)
    add_executable("${NAME}" ${SOURCES})
    target_link_libraries("${NAME}" ${LIBS})
    add_test(${NAME} ${NAME})
endmacro(create_test)


# Building application

# --- ThirdPartyLibs
add_subdirectory(./src/thirdPartyLibs)

# --- Components
add_subdirectory(./src/components)

# --- Main application
add_subdirectory(./src/appMain)

# Building tests
option(BUILD_TESTS "Possibility to build and run tests" OFF)
if(BUILD_TESTS)
    enable_testing() 
    include(Dart)
    # Framework GoogleTest is also integrated together gmock
    # and must not be added separately
    add_subdirectory(./src/thirdPartyLibs/gmock-1.6.0)
    add_subdirectory(./test)
endif()


# Building documentation

# At first creating directory for generated documentation. Unfortunately doxygen
# cannot generate it byself
FIND_PACKAGE(Doxygen)
  IF(DOXYGEN_FOUND)
    option(DOXYGEN_ENABLE_DIAGRAMS "Enable graphical diagram generation" ON)

    if(DOXYGEN_ENABLE_DIAGRAMS)
      set(DOXYGEN_ENABLE_DIAGRAMS_PARAM "YES")
    else(DOXYGEN_ENABLE_DIAGRAMS)
      set(DOXYGEN_ENABLE_DIAGRAMS_PARAM "NO")
    endif()
    configure_file("${PROJECT_SOURCE_DIR}/Doxyfile" "${PROJECT_BINARY_DIR}/Doxyfile")
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/doc/doxygen")
    ADD_CUSTOM_TARGET(doxygen COMMAND ${DOXYGEN_EXECUTABLE} "${PROJECT_BINARY_DIR}/Doxyfile")
  ELSE(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen not found. Documentation will not be generated")
    MESSAGE(STATUS "To enable documentation generation please install doxygen and graphviz packages")
    MESSAGE(STATUS "sudo apt-get install doxygen graphviz")
    MESSAGE(STATUS "To enable processing of MscGen comments please install mscgen")
    MESSAGE(STATUS "sudo apt-get install mscgen")
ENDIF(DOXYGEN_FOUND)


