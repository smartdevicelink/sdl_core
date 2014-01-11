# --- RequestWatchdog
add_subdirectory(./request_watchdog)

# --- TransportManager
#add_subdirectory(./transport_manager)

# --- JSONHandler
#add_subdirectory(./json_handler)

# --- SmartObjects
#add_subdirectory(./smart_objects)

# --- AppMgr
<<<<<<< HEAD
add_subdirectory(./application_manager)
=======
 add_subdirectory(./application_manager)
>>>>>>> 82329753b45fc633628550ff98db97fd014a218f

# --- ProtocolHandler
add_subdirectory(./protocol_handler)

# --- ProtocolHandler
#add_subdirectory(./connection_handler)

# --- Utils
#add_subdirectory(./utils)

# --- Mobile Message Handler
# add_subdirectory(./mobile_message_handler)

# --- Audio Manager
#add_subdirectory(./media_manager)

# --- policies (Policy Manager)
#add_subdirectory(./policies)

# --- DBus
IF (${QT_HMI})
    add_subdirectory(./dbus)
    add_subdirectory(./qt_hmi)
ENDIF (${QT_HMI})
