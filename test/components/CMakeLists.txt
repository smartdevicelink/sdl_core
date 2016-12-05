
# --- TransportManager
add_subdirectory(./transport_manager)

# --- JSONHandler
add_subdirectory(./json_handler)

# --- SmartObjects
#add_subdirectory(./smart_objects)

# --- AppMgr
#add_subdirectory(./application_manager)

if(ENABLE_SECURITY)
# --- ProtocolHandler
#add_subdirectory(./protocol_handler)
# --- SecurityManager
#add_subdirectory(./security_manager)
# TODO(EZamakhov): split connection tests and security
# --- ConnectionHandler
#add_subdirectory(./connection_handler)
endif()

add_subdirectory(./hmi_message_handler)

# --- Mobile Message Handler
# add_subdirectory(./mobile_message_handler)

# --- Audio Manager
#add_subdirectory(./media_manager)

# -- rpc_base
#add_subdirectory(./rpc_base)

# --- Functional module
add_subdirectory(./functional_module)

# --- CAN Module
add_subdirectory(./can_cooperation)

# --- DBus
if(${QT_HMI})
    add_subdirectory(./dbus)
    add_subdirectory(./qt_hmi)
endif(${QT_HMI})
