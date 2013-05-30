include_directories (
  ./include/
  ../utils/include/
  ../JSONHandler/include/
  ../protocol_handler/include/
  ../SmartObjects/include/
  ../hmi_message_handler/include
  ../mobile_message_handler/include
  ../connection_handler/include/
  ../request_watchdog/include/
  ../TransportManager/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ${CMAKE_BINARY_DIR}/src/components/Interfaces
)

set (SOURCES
./src/application_manager_impl.cc
./src/message.cc
./src/message_chaining.cc
./src/application_impl.cc
./src/basic_command_factory.cc
./src/commands/command_impl.cc
./src/commands/command_request_impl.cc
./src/commands/command_response_impl.cc
./src/commands/get_device_list_command.cc
./src/commands/add_command_request.cc
./src/commands/add_command_response.cc
#./src/commands/get_device_list_response_command.cc
./src/commands/register_app_interface_command.cc
./src/commands/register_app_interface_response_command.cc
./src/commands/generic_response_command.cc
./src/commands/unregister_app_interface_command.cc
./src/commands/show_command.cc
./src/commands/show_response_command.cc
./src/commands/speak_command.cc
./src/commands/speak_response_command.cc
./src/commands/set_global_properties_command.cc
./src/commands/set_global_properties_response_command.cc
./src/commands/set_media_clock_timer_command.cc
./src/commands/set_media_clock_timer_response_command.cc
./src/commands/encoded_sync_pdata_command.cc
./src/commands/encoded_sync_pdata_response_command.cc
./src/commands/dial_number_command.cc
./src/commands/reset_global_properties_command.cc
./src/commands/reset_global_properties_response_command.cc
./src/commands/unregister_app_interface_response_command.cc
./src/message_conversion.cc
./src/commands/delete_sub_menu_command.cc
./src/commands/delete_sub_menu_response_command.cc
./src/commands/add_sub_menu_command.cc
./src/commands/add_sub_menu_response_command.cc
./src/commands/delete_interaction_choisce_set_command.cc
./src/commands/delete_interaction_choice_set_response_command.cc
./src/commands/delete_command_request.cc
./src/commands/delete_command_response.cc
./src/commands/alert_command.cc
./src/commands/alert_response_command.cc
./src/commands/perform_audio_pass_thru_command.cc
./src/commands/perform_audio_pass_thru_response_command.cc
./src/commands/create_interaction_choice_set_command.cc
./src/commands/create_interaction_choice_set_command_response.cc
./src/commands/perform_interaction_command.cc
./src/commands/perform_interaction_response_command.cc
./src/commands/end_audio_pass_thru_command.cc
./src/commands/end_audio_pass_thru_response_command.cc
./src/commands/put_file_command.cc
./src/commands/put_file_response_command.cc
./src/commands/on_audio_pass_thru_command.cc
)


add_library("ApplicationManager" ${SOURCES} )
target_link_libraries ("ApplicationManager" log4cxx)
