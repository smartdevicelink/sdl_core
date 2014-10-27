# This file is copyrighted under the BSD-license for buildsystem files of KDE
# copyright 2010, Patrick Spendrin <ps_ml@gmx.de>

project(expat)

cmake_minimum_required(VERSION 2.6)
set(PACKAGE_BUGREPORT "expat-bugs@libexpat.org")
set(PACKAGE_NAME "expat")
set(PACKAGE_VERSION "2.1.0")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE_NAME}")

option(BUILD_tools "build the xmlwf tool for expat library" OFF)
option(BUILD_examples "build the examples for expat library" OFF)
option(BUILD_tests "build the tests for expat library" OFF)
option(BUILD_shared "build a shared expat library" ON)

# configuration options
set(XML_CONTEXT_BYTES 1024 CACHE STRING "Define to specify how much context to retain around the current parse point")
option(XML_DTD "Define to make parameter entity parsing functionality available" ON)
option(XML_NS "Define to make XML Namespaces functionality available" ON)

if(XML_DTD)
    set(XML_DTD 1)
else(XML_DTD)
    set(XML_DTD 0)
endif(XML_DTD)
if(XML_NS)
    set(XML_NS 1)
else(XML_NS)
    set(XML_NS 0)
endif(XML_NS)

if(BUILD_tests)
    enable_testing()
endif(BUILD_tests)

include(ConfigureChecks.cmake)

set(EXPAT_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXPAT_INCLUDE_DIRECTORY ${EXPAT_INCLUDE_DIRECTORY} PARENT_SCOPE)

set(EXPAT_LIBS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(EXPAT_LIBS_DIRECTORY ${EXPAT_LIBS_DIRECTORY} PARENT_SCOPE)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${EXPAT_INCLUDE_DIRECTORY})
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -wd4996)
endif(MSVC)

set(expat_SRCS
    lib/xmlparse.c
    lib/xmlrole.c
    lib/xmltok.c 
    lib/xmltok_impl.c 
    lib/xmltok_ns.c
)

if(WIN32 AND BUILD_shared)
    set(expat_SRCS ${expat_SRCS} lib/libexpat.def)
endif(WIN32 AND BUILD_shared)

if(BUILD_shared)
    set(_SHARED SHARED)
else(BUILD_shared)
    set(_SHARED STATIC)
endif(BUILD_shared)

add_library(expat ${_SHARED} ${expat_SRCS})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/libexpat.so
  DESTINATION ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib
)

set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix "\${prefix}/bin")
set(libdir "\${prefix}/lib")
set(includedir "\${prefix}/include")
configure_file(expat.pc.in ${CMAKE_CURRENT_BINARY_DIR}/expat.pc)

if(BUILD_tools AND NOT WINCE)
    set(xmlwf_SRCS
        xmlwf/xmlwf.c
        xmlwf/xmlfile.c
        xmlwf/codepage.c
        xmlwf/readfilemap.c
    )

    add_executable(xmlwf ${xmlwf_SRCS})
    target_link_libraries(xmlwf expat)
    install(TARGETS xmlwf DESTINATION bin)
    install(FILES doc/xmlwf.1 DESTINATION share/man/man1)
endif(BUILD_tools AND NOT WINCE)

if(BUILD_examples)
    add_executable(elements examples/elements.c)
    target_link_libraries(elements expat)

    add_executable(outline examples/outline.c)
    target_link_libraries(outline expat)
endif(BUILD_examples)

if(BUILD_tests)
    ## these are unittests that can be run on any platform
    add_executable(runtests tests/runtests.c tests/chardata.c tests/minicheck.c)
    target_link_libraries(runtests expat)
    add_test(runtests runtests)

    add_executable(runtestspp tests/runtestspp.cpp tests/chardata.c tests/minicheck.c)
    target_link_libraries(runtestspp expat)
    add_test(runtestspp runtestspp)
endif(BUILD_tests)
