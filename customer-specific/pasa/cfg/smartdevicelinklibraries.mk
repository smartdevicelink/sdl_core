# TODO make this more functionally oriented.
#  That is to make them more like the types ... so client libraries, etc.
	
SMARTDEVICELINK_STATIC_LIBS = \
	encryption$(DEBUG_EXT) \
	jsoncpp$(DEBUG_EXT) \
	MessageBroker$(DEBUG_EXT) \
	MessageBrokerClient$(DEBUG_EXT) \
	MessageBrokerServer$(DEBUG_EXT) \
	utils$(DEBUG_EXT) \
	smart_objects$(DEBUG_EXT) \
	config_profile$(DEBUG_EXT) \
	formatters$(DEBUG_EXT) \
	interfaces$(DEBUG_EXT) \
	request_watchdog$(DEBUG_EXT) \
	hmi_message_handler$(DEBUG_EXT) \
	connection_handler$(DEBUG_EXT) \
	protocol_handler$(DEBUG_EXT) \
	transport_manager$(DEBUG_EXT) \
	policies(DEBUG_EXT) \
	application_manager$(DEBUG_EXT) \	
		
SMARTDEVICELINK_DYNAMIC_LIBS = \
	$(TEAM_ROOT)SmartDeviceLink/dll/$(TARGET)$(BUILD_CFG)apr-1$(DEBUG_EXT) \
	$(TEAM_ROOT)SmartDeviceLink/dll/$(TARGET)$(BUILD_CFG)expat$(DEBUG_EXT) \
	$(TEAM_ROOT)SmartDeviceLink/dll/$(TARGET)$(BUILD_CFG)aprutil-1$(DEBUG_EXT) \
	$(TEAM_ROOT)SmartDeviceLink/dll/$(TARGET)$(BUILD_CFG)log4cxx$(DEBUG_EXT) \
