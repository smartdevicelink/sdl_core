# --- Check libavahi-common, libavahi-client availability

FIND_LIBRARY(
             LIBAVAHI_CLIENT
             NAMES libavahi-client.so
             PATHS /usr/local/lib /usr/lib /opt/local/include /usr/lib/i386-linux-gnu/
            )       
                
if (NOT LIBAVAHI_CLIENT)
	message(ERROR " libavahi-client is not found")
    message(ERROR " To install it type in the command line:")
    message(ERROR " sudo apt-get install libavahi-client-dev")
endif (NOT LIBAVAHI_CLIENT)

FIND_LIBRARY(
             LIBAVAHI_COMMON
             NAMES libavahi-common.so
             PATHS /usr/local/lib /usr/lib /opt/local/include /usr/lib/i386-linux-gnu/
            )       
     
if (NOT LIBAVAHI_COMMON)
	message(ERROR " libavahi-common is not found")
    message(ERROR " To install it type in the command line:")
    message(ERROR " sudo apt-get install libavahi-common-dev")
endif (NOT LIBAVAHI_COMMON)