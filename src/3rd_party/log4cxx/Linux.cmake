
  set(CONFIGURE_FLAGS
    "--with-apr=../apr/apr-build"
    "--with-apr-util=../apr-util-build"
    "LDFLAGS=-L${EXPAT_LIBS_DIRECTORY}"
  )

  add_custom_command(OUTPUT ${LOG4CXX_BUILD_DIRECTORY}/Makefile
    COMMAND CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${LOG4CXX_SOURCE_DIRECTORY}/configure ${CONFIGURE_FLAGS}
    DEPENDS libapr-1
    DEPENDS apr-util
    WORKING_DIRECTORY ${LOG4CXX_BUILD_DIRECTORY}
  )

add_custom_target(liblog4cxx ALL make
  COMMAND /bin/bash -c \"
	cd ${CMAKE_CURRENT_SOURCE_DIR}\;
	git log . 1>/dev/null 2>&1\;
        if [ \\$$? == 0 ]; then
          grep \\".commit_hash\\" ${LOG4CXX_LIBS_DIRECTORY}/liblog4cxx.so 1>/dev/null 2>&1\;
          if [ ! \\\$$? == 0 ]\; then
            cd ${CMAKE_CURRENT_SOURCE_DIR} &&
            git log --pretty=\\"format:%H\\" -1 ${LOG4CXX_SOURCE_DIRECTORY} > /tmp/commit_hash 2>/dev/null &&
            echo \\"Adding .commit_hash section\\" &&
            objcopy --add-section .commit_hash=/tmp/commit_hash ${LOG4CXX_LIBS_DIRECTORY}/liblog4cxx.so ${LOG4CXX_LIBS_DIRECTORY}/liblog4cxx.so 1>/dev/null 2>&1\;
          fi;
        fi\"
  DEPENDS ${LOG4CXX_BUILD_DIRECTORY}/Makefile
  WORKING_DIRECTORY ${LOG4CXX_BUILD_DIRECTORY}
)
