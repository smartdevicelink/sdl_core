# --- jsoncpp
add_subdirectory(./jsoncpp)

# --- MessageBroker
add_subdirectory(./MessageBroker)

# --- encryption
add_subdirectory(./encryption)

if (BUILD_USB_SUPPORT)
# --- LibUSB-1.0.16
#add_subdirectory(./libusbx-1.0.16)
endif()
