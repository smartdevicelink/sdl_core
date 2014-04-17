include_directories (
  ./include/time_tester
  ../utils/include/
  ../protocol_handler/include/
  ../connection_handler/include/
  ../transport_manager/include/
  ../application_manager/include/
  ../hmi_message_handler/include/
  ../request_watchdog/include/
  ../formatters/include/
  ../media_manager/include/
  ../smart_objects/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ${CMAKE_BINARY_DIR}/src/components/
)

set (SOURCES
    ./src/time_manager.cc
    ./src/application_manager_observer.cc
    ./src/transport_manager_observer.cc
    ./src/application_manager_metric.cc
    ./src/transport_manager_metric.cc


)

add_library("TimeTester" ${SOURCES})
target_link_libraries("TimeTester" ${LIBRARIES})
