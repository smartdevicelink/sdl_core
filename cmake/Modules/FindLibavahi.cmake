# --- Check libavahi-common, libavahi-client availability

FIND_LIBRARY(
             LIBAVAHI_CLIENT
             NAMES libavahi-client.so
             PATHS /usr/local/lib /usr/lib /opt/local/include /usr/lib/i386-linux-gnu/
            )       
                
if (NOT LIBAVAHI_CLIENT)
    message(STATUS " libavahi-client is not found")
    message(STATUS " To install it type in the command line:")
    message(STATUS " sudo apt-get install libavahi-client-dev")
endif (NOT LIBAVAHI_CLIENT)

FIND_LIBRARY(
             LIBAVAHI_COMMON
             NAMES libavahi-common.so
             PATHS /usr/local/lib /usr/lib /opt/local/include /usr/lib/i386-linux-gnu/
            )       
     
if (NOT LIBAVAHI_COMMON)
    message(STATUS " libavahi-common is not found")
    message(STATUS " To install it type in the command line:")
    message(STATUS " sudo apt-get install libavahi-common-dev")
endif (NOT LIBAVAHI_COMMON)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
if (NOT LIBAVAHI_COMMON OR NOT LIBAVAHI_CLIENT)
  message(FATAL_ERROR "One of necesary component of libavahi is not found. Exiting!")
endif(NOT LIBAVAHI_COMMON OR NOT LIBAVAHI_CLIENT)
endif()