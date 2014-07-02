project(test)

add_definitions(${DBUS_INTERNAL_CLIENT_DEFINITIONS})

add_library(dbus-testutils STATIC
    ${CMAKE_SOURCE_DIR}/../test/test-utils.h
    ${CMAKE_SOURCE_DIR}/../test/test-utils.c
)
target_link_libraries(dbus-testutils ${DBUS_INTERNAL_LIBRARIES})

add_subdirectory( name-test )

set (test-service_SOURCES
    ${CMAKE_SOURCE_DIR}/../test/test-service.c
)

set (test-names_SOURCES
    ${CMAKE_SOURCE_DIR}/../test/test-names.c
)

set (break_loader_SOURCES
    ${CMAKE_SOURCE_DIR}/../test/break-loader.c
)

set (test-shell-service_SOURCES
    ${CMAKE_SOURCE_DIR}/../test/test-shell-service.c
)

set (shell-test_SOURCES
        ${CMAKE_SOURCE_DIR}/../test/shell-test.c
)

set (spawn-test_SOURCES
    ${CMAKE_SOURCE_DIR}/../test/spawn-test.c
)

set (test-exit_SOURCES
    ${CMAKE_SOURCE_DIR}/../test/test-exit.c
)

set (test-segfault_SOURCES
    ${CMAKE_SOURCE_DIR}/../test/test-segfault.c
)

set (test-sleep-forever_SOURCES
    ${CMAKE_SOURCE_DIR}/../test/test-sleep-forever.c
)

add_executable(test-service ${test-service_SOURCES})
target_link_libraries(test-service dbus-testutils)

add_executable(test-names ${test-names_SOURCES})
target_link_libraries(test-names dbus-testutils)

add_executable(shell-test ${shell-test_SOURCES})
target_link_libraries(shell-test ${DBUS_INTERNAL_LIBRARIES})
ADD_TEST(shell-test ${EXECUTABLE_OUTPUT_PATH}/shell-test${EXEEXT})

add_executable(test-shell-service ${test-shell-service_SOURCES})
target_link_libraries(test-shell-service dbus-testutils)

add_executable(spawn-test ${spawn-test_SOURCES})
target_link_libraries(spawn-test ${DBUS_INTERNAL_LIBRARIES})

add_executable(test-exit ${test-exit_SOURCES})
target_link_libraries(test-exit ${DBUS_INTERNAL_LIBRARIES})

add_executable(test-segfault ${test-segfault_SOURCES})
target_link_libraries(test-segfault ${DBUS_INTERNAL_LIBRARIES})

add_executable(test-sleep-forever ${test-sleep-forever_SOURCES})
target_link_libraries(test-sleep-forever ${DBUS_INTERNAL_LIBRARIES})

### keep these in creation order, i.e. uppermost dirs first 
set (TESTDIRS
    test/data
    test/data/valid-messages
    test/data/invalid-messages
    test/data/incomplete-messages
    test/data/auth
    test/data/sha-1
    test/data/valid-config-files
    test/data/valid-config-files/basic.d
    test/data/valid-config-files/system.d
    test/data/valid-config-files-system
    test/data/valid-introspection-files
    test/data/valid-messages
    test/data/valid-service-files
    test/data/valid-service-files-system
    test/data/invalid-config-files
    test/data/invalid-config-files-system
    test/data/invalid-messages
    test/data/invalid-service-files-system
    test/data/equiv-config-files
    test/data/equiv-config-files/basic
    test/data/equiv-config-files/basic/basic.d
    test/data/equiv-config-files/entities
    test/data/equiv-config-files/entities/basic.d
)
set (CONFIG_VERBOSE 0)

### copy tests to builddir so that generated tests and static tests 
### are all in one place.
MESSAGE(STATUS "Copying test files to test directory")
FOREACH(FILE_TYPE *.message *.message-raw *.auth-script *.sha1 *.txt *.conf *.service)
    FOREACH(DIR ${TESTDIRS})
        FILE(GLOB FILES "${CMAKE_SOURCE_DIR}/../${DIR}/${FILE_TYPE}" )
        FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${DIR})
        FOREACH(FILE ${FILES})
            GET_FILENAME_COMPONENT(FILENAME ${FILE} NAME)
            SET (TARGET ${CMAKE_BINARY_DIR}/${DIR}/${FILENAME})
            configure_file(${FILE} ${TARGET} COPYONLY)
            IF (CONFIG_VERBOSE)
                MESSAGE("${FILE}")
            ENDIF (CONFIG_VERBOSE)
        ENDFOREACH(FILE)
    ENDFOREACH(DIR)
ENDFOREACH(FILE_TYPE)

### generate test files
MESSAGE(STATUS "Generating test files from templates into test directory")

FOREACH(FILE_TYPE *.conf.in *.service.in)
  FOREACH(DIR ${TESTDIRS})
    FILE(GLOB FILES "${CMAKE_SOURCE_DIR}/../${DIR}/${FILE_TYPE}" )
    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${DIR})
    FOREACH(FILE ${FILES})
      GET_FILENAME_COMPONENT(FILENAME ${FILE} NAME)
      STRING(REGEX REPLACE "\\.in$" "" FILENAME ${FILENAME})
      SET (TARGET ${CMAKE_BINARY_DIR}/${DIR}/${FILENAME})
      configure_file(${FILE} ${TARGET} @ONLY IMMEDIATE)
      IF (CONFIG_VERBOSE)
        MESSAGE("${FILE}")
      ENDIF (CONFIG_VERBOSE)
    ENDFOREACH(FILE)
  ENDFOREACH(DIR)
ENDFOREACH(FILE_TYPE)

MESSAGE(STATUS "Copying generated bus config files to test directory")
set (OUTDIR ${CMAKE_BINARY_DIR}/test/data/valid-config-files)

FILE(GLOB FILES "${CMAKE_BINARY_DIR}/bus/*.conf" )
FILE(MAKE_DIRECTORY ${OUTDIR})
FOREACH(FILE ${FILES})
    GET_FILENAME_COMPONENT(FILENAME ${FILE} NAME)
    SET (TARGET ${OUTDIR}/${FILENAME})
    configure_file(${FILE} ${TARGET} COPYONLY)
    IF (CONFIG_VERBOSE)
        MESSAGE("FROM: ${FILE}\nTO: ${TARGET}\n")
    ENDIF (CONFIG_VERBOSE)
ENDFOREACH(FILE)

# todo: for installation the TEST_..._BINARY variables must reflect the 
#       installation dir or has to be defined relative
#
