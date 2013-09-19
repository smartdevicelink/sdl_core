# --- Check QT5 necessary libraries

FIND_LIBRARY(
             QT5Libs
             NAMES libQt5Core.so
		   libQt5Quick.so
             PATHS /usr/local/lib /usr/lib /opt/local/include /usr/lib/i386-linux-gnu/
            )       
                
if (NOT QT5Libs)
    message(STATUS "Necessary QT5 libraries are not found")
    message(STATUS "To install it type in the command line:")
    message(STATUS "sudo apt-get install qtdeclarative5-dev")
endif (NOT QT5Libs)