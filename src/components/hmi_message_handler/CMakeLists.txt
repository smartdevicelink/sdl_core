include_directories (
  ./include/
  ../application_manager/include/
  ../utils/include/
  ../smart_objects/include/
  ../formatters/include/
  ../config_profile/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/MessageBroker/include
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include/
  ${CMAKE_SOURCE_DIR}/
)

set (SOURCES
./src/hmi_message_handler_impl.cc
./src/messagebroker_adapter.cc
./src/hmi_message_adapter.cc
./src/to_hmi_thread_impl.cc
./src/from_hmi_thread_impl.cc
./src/dbus_message_adapter.cc
)

IF (${QT_HMI})
    set (DBUS_ADAPTER DBus)
ENDIF (${QT_HMI})

set (LIBRARIES
    log4cxx
    ${DBUS_ADAPTER}
)

add_library("HMIMessageHandler" ${SOURCES})
target_link_libraries ("HMIMessageHandler" ${LIBRARIES})
