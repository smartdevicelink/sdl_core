# --- Interfaces
add_subdirectory(./interfaces)

# --- TransportManager
if(BUILD_ONLY_TM)
add_subdirectory(./transport_manager)
add_subdirectory(./protocol_handler)
else()
add_subdirectory(./TransportManager)
endif()

# --- formatters
add_subdirectory(./formatters)

add_subdirectory(./connection_handler)

# --- Utils
add_subdirectory(./utils)

# --- SmartObjects
add_subdirectory(./SmartObjects)

# --- Mobile message handler
add_subdirectory(./mobile_message_handler)

# --- Application manager
add_subdirectory(./application_manager)

# --- Request watchdog
add_subdirectory(./request_watchdog)

# --- HMI Message Handler
add_subdirectory(./hmi_message_handler)

# --- Config
add_subdirectory(./config_profile)
