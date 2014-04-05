# --- RequestWatchdog
#add_subdirectory(./request_watchdog)

# --- TransportManager
add_subdirectory(./transport_manager)

# --- JSONHandler
add_subdirectory(./json_handler)

# --- SmartObjects
add_subdirectory(./smart_objects)

# --- AppMgr
add_subdirectory(./application_manager)


# --- ProtocolHandler
add_subdirectory(./protocol_handler)

# --- ProtocolHandler
add_subdirectory(./connection_handler)

add_subdirectory(./hmi_message_handler)

# --- Utils
add_subdirectory(./utils)

# --- Mobile Message Handler
# add_subdirectory(./mobile_message_handler)

# --- Audio Manager
#add_subdirectory(./media_manager)

# -- rpc_base
add_subdirectory(./rpc_base)

# --- DBus
IF (${QT_HMI})
    add_subdirectory(./dbus)
    add_subdirectory(./qt_hmi)
ENDIF (${QT_HMI})
