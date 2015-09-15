include_directories (
  ./include
)

set (LIBUSB_SOURCES
	./src/bluetooth.c
	./src/hci.c
	./src/sdp.c
)

add_library("bluetooth" ${LIBUSB_SOURCES})

