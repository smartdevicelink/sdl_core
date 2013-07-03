# --- RequestWatchdog
if(NOT BUILD_ONLY_TM)
add_subdirectory(./request_watchdog)
endif()

# --- TransportManager
if(BUILD_ONLY_TM)
add_subdirectory(./transport_manager)
else()
add_subdirectory(./TransportManager)
endif()

# --- JSONHandler
if(NOT BUILD_ONLY_TM)
add_subdirectory(./json_handler)
endif()

# --- SmartObjects
if(NOT BUILD_ONLY_TM)
add_subdirectory(./SmartObjects)
endif()

# --- AppMgr
if(NOT BUILD_ONLY_TM)
 add_subdirectory(./application_manager)
endif()

# --- ProtocolHandler
if(NOT BUILD_ONLY_TM)
add_subdirectory(./protocol_handler)
endif()

# --- Utils
if(NOT BUILD_ONLY_TM)
add_subdirectory(./utils)
endif()

# --- Mobile Message Handler
if(NOT BUILD_ONLY_TM)
add_subdirectory(./mobile_message_handler)
endif()
