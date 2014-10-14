include_directories (
  ./include/
  ../include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/generic_module.cc
)

add_library("FunctionalModule" ${SOURCES})

if(ENABLE_LOG)
  target_link_libraries("FunctionalModule" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()
