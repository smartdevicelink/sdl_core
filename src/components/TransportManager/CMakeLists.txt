cmake_minimum_required(VERSION 2.8)
set (PROJECT TransportManager)
project (${PROJECT})

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

include_directories(./include
                    ../../thirdPartyLibs/logger/include/)

set (SOURCES ./src/IDeviceAdapter.cpp
             ./src/IDeviceAdapterListener.cpp
             ./src/IHandleGenerator.cpp
             ./src/ITransportManager.cpp
             ./src/ITransportManagerListener.cpp
             ./src/CBluetoothAdapter.cpp
             ./src/CTransportManager.cpp
             ./src/SInternalDeviceInfo.cpp)

add_library(${PROJECT} ${SOURCES})

install (TARGETS ${PROJECT} DESTINATION bin)


# Set of macros for processing and generating doxygen documentation
MACRO(CONFIGURE_DOXYGEN_FILE DOXYGEN_CONFIG_FILE FILE_NAME_SUFFIX)
 IF(EXISTS ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE})
    FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/doxy-${FILE_NAME_SUFFIX}.conf)
    FILE(READ ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE} DOXYFILE_CONTENTS)
     STRING(REGEX REPLACE ";" "\\\\;" DOXYFILE_CONTENTS "${DOXYFILE_CONTENTS}")
     STRING(REGEX REPLACE "\n" ";" DOXYFILE_LINES "${DOXYFILE_CONTENTS}")
     LIST(LENGTH DOXYFILE_LINES ROW)
     MATH(EXPR ROW "${ROW} - 1")
     FOREACH(I RANGE ${ROW})
        LIST(GET DOXYFILE_LINES ${I} LINE)
        IF(LINE STRGREATER "")
         STRING(REGEX MATCH "^[a-zA-Z]([^ ])+" DOXY_PARAM ${LINE})
         IF(DEFINED DOXY_${DOXY_PARAM})
            STRING(REGEX REPLACE "=([^\n])+" "= ${DOXY_${DOXY_PARAM}}" LINE ${LINE})
         ENDIF(DEFINED DOXY_${DOXY_PARAM})
        ENDIF()
        FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/doxy-${FILE_NAME_SUFFIX}.conf "${LINE}\n")
     ENDFOREACH()
 ELSE()
    MESSAGE(SEND_ERROR "Doxygen configuration file '${DOXYGEN_CONFIG_FILE}' not found. Documentation will not be generated")
 ENDIF()
ENDMACRO(CONFIGURE_DOXYGEN_FILE)


MACRO(ADD_DOCUMENTATION TARGET DOXYGEN_CONFIG_FILE)
 FIND_PACKAGE(Doxygen)
 IF(DOXYGEN_FOUND)
    CONFIGURE_DOXYGEN_FILE(${DOXYGEN_CONFIG_FILE} ${TARGET})
    ADD_CUSTOM_TARGET(${TARGET} COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxy-${TARGET}.conf)
 ELSE(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen not found. Documentation will not be generated")
    MESSAGE(STATUS "To enable documentation generation please install doxygen and graphviz packages")
 ENDIF(DOXYGEN_FOUND)
ENDMACRO(ADD_DOCUMENTATION)

# Adding additional "doc" target for building doxygen documentation
# Here any of the Doxygen configuration variables can be set by
# specifying corresponded variable with prefix DOXY

# At first creating directory for generated documentation. Unfortunately doxygen
# cannot generate it byself
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen")
SET(DOXY_INPUT "${PROJECT_SOURCE_DIR}")
SET(DOXY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen")
ADD_DOCUMENTATION(doc Doxyfile)
