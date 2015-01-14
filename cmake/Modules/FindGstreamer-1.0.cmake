FILE(TO_CMAKE_PATH "$ENV{GSTREAMER_DIR}" TRY1_DIR)
FILE(TO_CMAKE_PATH "${GSTREAMER_DIR}" TRY2_DIR)
FILE(GLOB GSTREAMER_DIR ${TRY1_DIR} ${TRY2_DIR})

FIND_PATH(GSTREAMER_gst_INCLUDE_DIR gst/gst.h
                                    PATHS ${GSTREAMER_DIR}/include /usr/local/include/gstreamer-1.0 /usr/include/gstreamer-1.0
                                    ENV INCLUDE DOC "Directory containing gst/gst.h include file")

FIND_PATH(GSTREAMER_gstconfig_INCLUDE_DIR gst/gstconfig.h
                                          PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/lib/include /usr/local/include/gstreamer-1.0 /usr/include/gstreamer-1.0 /usr/local/lib/include/gstreamer-1.0 /usr/lib/include/gstreamer-1.0
                                          ENV INCLUDE DOC "Directory containing gst/gstconfig.h include file")

FIND_LIBRARY(GSTREAMER_gstaudio_LIBRARY NAMES gstaudio-1.0 libgstaudio-1.0
                                        PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                        ENV LIB
                                        DOC "gstaudio library to link with"
                                        NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstapp_LIBRARY NAMES gstapp-1.0 libgstapp-1.0
                                        PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                        ENV LIB
                                        DOC "gstapp library to link with"
                                        NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstbase_LIBRARY NAMES gstbase-1.0 libgstbase-1.0
                                       PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                       ENV LIB
                                       DOC "gstbase library to link with"
                                       NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GLIB_gstcdda_LIBRARY NAMES gstcdda-1.0 libgstcdda-1.0
                                  PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                  ENV LIB
                                  DOC "gstcdda library to link with"
                                  NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstcontroller_LIBRARY NAMES gstcontroller-1.0 libgstcontroller-1.0
                                             PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                             ENV LIB
                                             DOC "gstcontroller library to link with"
                                             NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstdataprotocol_LIBRARY NAMES gstdataprotocol-1.0 libgstdataprotocol-1.0
                                               PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                               ENV LIB
                                               DOC "gstdataprotocol library to link with"
                                               NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstinterfaces_LIBRARY NAMES gstinterfaces-1.0 libgstinterfaces-1.0
                                             PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                             ENV LIB
                                             DOC "gstinterfaces library to link with"
                                             NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstnet_LIBRARY NAMES gstnet-1.0 libgstnet-1.0
                                      PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                      ENV LIB
                                      DOC "gstnet library to link with"
                                      NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstnetbuffer_LIBRARY NAMES gstnetbuffer-1.0 libgstnetbuffer-1.0
                                            PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                            ENV LIB
                                            DOC "gstnetbuffer library to link with"
                                            NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstpbutils_LIBRARY NAMES gstpbutils-1.0 libgstpbutils-1.0
                                          PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                          ENV LIB
                                          DOC "gstpbutils library to link with"
                                          NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstreamer_LIBRARY NAMES gstreamer-1.0 libgstreamer-1.0
                                         PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                         ENV LIB
                                         DOC "gstreamer library to link with"
                                         NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstriff_LIBRARY NAMES gstriff-1.0 libgstriff-1.0
                                             PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                             ENV LIB
                                             DOC "gstriff library to link with"
                                             NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstrtp_LIBRARY NAMES gstrtp-1.0 libgstrtp-1.0
                                    PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                    ENV LIB
                                    DOC "gstrtp library to link with"
                                    NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstrtsp_LIBRARY NAMES gstrtsp-1.0 libgstrtsp-1.0
                                       PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                       ENV LIB
                                       DOC "gstrtsp library to link with"
                                       NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstsdp_LIBRARY NAMES gstsdp-1.0 libgstsdp-1.0
                                      PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                      ENV LIB
                                      DOC "gstsdp library to link with"
                                      NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gsttag_LIBRARY NAMES gsttag-1.0 libgsttag-1.0
                                 PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                 ENV LIB
                                 DOC "gsttag library to link with"
                                 NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GSTREAMER_gstvideo_LIBRARY NAMES gstvideo-1.0 libgstvideo-1.0
                                        PATHS ${GSTREAMER_DIR}/bin ${GSTREAMER_DIR}/win32/bin ${GSTREAMER_DIR}/bin/bin C:/gstreamer/bin ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib /usr/local/lib /usr/lib
                                        ENV LIB
                                        DOC "gstvideo library to link with"
                                        NO_SYSTEM_ENVIRONMENT_PATH)


IF (GSTREAMER_gst_INCLUDE_DIR AND GSTREAMER_gstconfig_INCLUDE_DIR AND
    GSTREAMER_gstaudio_LIBRARY AND GSTREAMER_gstbase_LIBRARY AND GSTREAMER_gstcontroller_LIBRARY AND
    GSTREAMER_gstdataprotocol_LIBRARY AND GSTREAMER_gstinterfaces_LIBRARY AND GSTREAMER_gstnet_LIBRARY AND
    GSTREAMER_gstnetbuffer_LIBRARY AND GSTREAMER_gstpbutils_LIBRARY AND GSTREAMER_gstreamer_LIBRARY AND
    GSTREAMER_gstriff_LIBRARY AND GSTREAMER_gstrtp_LIBRARY AND GSTREAMER_gstrtsp_LIBRARY AND GSTREAMER_gstsdp_LIBRARY AND
    GSTREAMER_gsttag_LIBRARY AND GSTREAMER_gstvideo_LIBRARY)
  SET(GSTREAMER_INCLUDE_DIR ${GSTREAMER_gst_INCLUDE_DIR} ${GSTREAMER_gstconfig_INCLUDE_DIR})
  list(REMOVE_DUPLICATES GSTREAMER_INCLUDE_DIR)
  SET(GSTREAMER_LIBRARIES ${GSTREAMER_gstaudio_LIBRARY} ${GSTREAMER_gstbase_LIBRARY}
                          ${GSTREAMER_gstcontroller_LIBRARY} ${GSTREAMER_gstdataprotocol_LIBRARY} ${GSTREAMER_gstinterfaces_LIBRARY}
                          ${GSTREAMER_gstnet_LIBRARY} ${GSTREAMER_gstnetbuffer_LIBRARY} ${GSTREAMER_gstpbutils_LIBRARY}
                          ${GSTREAMER_gstreamer_LIBRARY} ${GSTREAMER_gstriff_LIBRARY} ${GSTREAMER_gstrtp_LIBRARY}
                          ${GSTREAMER_gstrtsp_LIBRARY} ${GSTREAMER_gstsdp_LIBRARY} ${GSTREAMER_gsttag_LIBRARY} ${GSTREAMER_gstvideo_LIBRARY})
  list(REMOVE_DUPLICATES GSTREAMER_LIBRARIES)
  SET(GSTREAMER_FOUND TRUE)
ENDIF (GSTREAMER_gst_INCLUDE_DIR AND GSTREAMER_gstconfig_INCLUDE_DIR AND
       GSTREAMER_gstaudio_LIBRARY AND GSTREAMER_gstbase_LIBRARY AND GSTREAMER_gstcontroller_LIBRARY AND
       GSTREAMER_gstdataprotocol_LIBRARY AND GSTREAMER_gstinterfaces_LIBRARY AND GSTREAMER_gstnet_LIBRARY AND
       GSTREAMER_gstnetbuffer_LIBRARY AND GSTREAMER_gstpbutils_LIBRARY AND GSTREAMER_gstreamer_LIBRARY AND
       GSTREAMER_gstriff_LIBRARY AND GSTREAMER_gstrtp_LIBRARY AND GSTREAMER_gstrtsp_LIBRARY AND GSTREAMER_gstsdp_LIBRARY AND
       GSTREAMER_gsttag_LIBRARY AND GSTREAMER_gstvideo_LIBRARY)
