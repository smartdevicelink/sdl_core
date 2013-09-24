include_directories (
  ./include/
  ../jsoncpp/include
)

set (MESSAGE_BROKER_SOURCES
  ./src/lib_messagebroker/CMessageBroker.cpp
  ./src/lib_messagebroker/CMessageBrokerRegistry.cpp
  ./src/lib_messagebroker/md5.cpp
  ./src/lib_messagebroker/system.cpp
  ./src/lib_messagebroker/websocket_handler.cpp
)

set (MESSAGE_BROKER_CLIENT_SOURCES
  ./src/client/mb_client.cpp
  ./src/client/mb_tcpclient.cpp
  ./src/client/mb_controller.cpp
)

set (MESSAGE_BROKER_SERVER_SOURCES
  ./src/server/mb_server.cpp
  ./src/server/mb_tcpserver.cpp
  ./src/server/networking.cpp
)

add_library("MessageBroker" ${MESSAGE_BROKER_SOURCES})

IF (${WEB_HMI})
add_library("MessageBrokerClient" ${MESSAGE_BROKER_CLIENT_SOURCES})
target_link_libraries("MessageBrokerClient" "MessageBroker")

add_library("MessageBrokerServer" ${MESSAGE_BROKER_SERVER_SOURCES})
target_link_libraries("MessageBrokerServer" "MessageBroker")
ENDIF (${WEB_HMI})