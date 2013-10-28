# --- RequestWatchdog
add_subdirectory(./request_watchdog)

# --- TransportManager
add_subdirectory(./transport_manager)

# --- JSONHandler
# add_subdirectory(./formatters)

# --- SmartObjects
add_subdirectory(./smart_objects)

# --- AppMgr
 add_subdirectory(./application_manager)

# --- ProtocolHandler
add_subdirectory(./protocol_handler)

# --- Utils
add_subdirectory(./utils)

# --- Mobile Message Handler
#add_subdirectory(./mobile_message_handler)

# --- Audio Manager
add_subdirectory(./media_manager)

# --- HMI Message Handler
add_subdirectory(./hmi_message_handler)

# --- DBus
IF (${QT_HMI})
    add_subdirectory(./dbus)
ENDIF (${QT_HMI})
