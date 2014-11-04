include_directories (
  ./include/
  ../include/
  ../functional_module/include/
  ../application_manager/include
  ../connection_handler/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
)

set (SOURCES
    ./src/can_module.cc
    ./src/can_app_extension.cc
    ./src/can_tcp_connection.cc
)

add_library("CANCooperation" ${SOURCES})

if(ENABLE_LOG)
  target_link_libraries("CANCooperation" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()
