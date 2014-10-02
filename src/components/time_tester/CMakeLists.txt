include_directories (
  ./include/time_tester
  ../utils/include/
  ../protocol_handler/include/
  ../connection_handler/include/
  ../transport_manager/include/
  ../application_manager/include/
  ../hmi_message_handler/include/
  ../formatters/include/
  ../media_manager/include/
  ../smart_objects/include/
  ../config_profile/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/metric_wrapper.cc
    ./src/time_manager.cc
    ./src/application_manager_observer.cc
    ./src/transport_manager_observer.cc
    ./src/protocol_handler_observer.cc
    ./src/application_manager_metric.cc
    ./src/transport_manager_metric.cc
    ./src/protocol_handler_metric.cc
)

add_library("TimeTester" ${SOURCES})
target_link_libraries("TimeTester" ${LIBRARIES})
