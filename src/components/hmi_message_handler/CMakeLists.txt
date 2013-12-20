include_directories (
  ./include/
  ../application_manager/include/
  ../utils/include/
  ../smart_objects/include/
  ../formatters/include/
  ../config_profile/include/
  ../media_manager/include/
  ../protocol_handler/include
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/MessageBroker/include
  ../config_profile/include
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include/
  ${CMAKE_SOURCE_DIR}/
)

set (DBUS_SOURCE)
IF (${QT_HMI})
set (DBUS_SOURCE ./src/dbus_message_adapter.cc)
ENDIF (${QT_HMI})
set (SOURCES
./src/hmi_message_handler_impl.cc
./src/messagebroker_adapter.cc
./src/hmi_message_adapter.cc
${DBUS_SOURCE}
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
