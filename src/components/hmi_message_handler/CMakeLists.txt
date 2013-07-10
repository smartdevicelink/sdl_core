include_directories (
  ./include/
  ../application_manager/include/
  ../utils/include/
  ../config_profile/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/MessageBroker/include
  ${CMAKE_BINARY_DIR}/src/components/Interfaces/
)

set (SOURCES
./src/hmi_message_handler_impl.cc
./src/messagebroker_adapter.cc
./src/hmi_message_adapter.cc
./src/to_hmi_thread_impl.cc
./src/from_hmi_thread_impl.cc
)


add_library("HMIMessageHandler" ${SOURCES} )
target_link_libraries ("HMIMessageHandler" log4cxx)
