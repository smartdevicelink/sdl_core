include_directories (
  ./include/
  ../utils/include/
  ../application_manager/include/
  ../protocol_handler/include/
  ../transport_manager/include/
  ../smart_objects/include/
)

set (SOURCES
./src/mobile_message_handler_impl.cc
./src/messages_from_mobile_app_handler.cc
./src/message_to_mobile_app_handler.cc
)


add_library("MobileMessageHandler" ${SOURCES} )
target_link_libraries ("MobileMessageHandler" log4cxx)
