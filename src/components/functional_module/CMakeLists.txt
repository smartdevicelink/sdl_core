include_directories (
  ./include/
  ../include/
  ../application_manager/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/generic_module.cc
    ./src/plugin_manager.cc
)

add_library("FunctionalModule" ${SOURCES})

if(ENABLE_LOG)
  target_link_libraries("FunctionalModule" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()
