FILE(TO_CMAKE_PATH "$ENV{GLIB2_DIR}" TRY1_DIR)
FILE(TO_CMAKE_PATH "${GLIB2_DIR}" TRY2_DIR)
FILE(GLOB GLIB2_DIR ${TRY1_DIR} ${TRY2_DIR})

FIND_PATH(GLIB_glib_2_INCLUDE_DIR glib.h
                                  PATHS ${GLIB2_DIR}/include ${GLIB2_DIR}/include/glib-2.0 /usr/local/include/glib-2.0 /usr/include/glib-2.0 /usr/lib/glib-2.0/include/
                                  ENV INCLUDE DOC "Directory containing glib.h include file")

FIND_PATH(GLIB_glibconfig_2_INCLUDE_DIR glibconfig.h
                                        PATHS ${GLIB2_DIR}/include ${GLIB2_DIR}/include/glib-2.0 ${GLIB2_DIR}/lib/include ${GLIB2_DIR}/lib/glib-2.0/include /usr/local/include/glib-2.0 /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/local/lib/glib-2.0/include
                                        ENV INCLUDE DOC "Directory containing glibconfig.h include file")

FIND_LIBRARY(GLIB_glib_2_LIBRARY NAMES glib-2.0
                                 PATHS ${GLIB2_DIR}/bin ${GLIB2_DIR}/win32/bin ${GLIB2_DIR}/lib ${GLIB2_DIR}/win32/lib /usr/local/lib /usr/lib
                                 ENV LIB
                                 DOC "glib library to link with"
                                 NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GLIB_gmodule_2_LIBRARY NAMES gmodule-2.0
                                    PATHS ${GLIB2_DIR}/bin ${GLIB2_DIR}/win32/bin ${GLIB2_DIR}/lib ${GLIB2_DIR}/win32/lib /usr/local/lib /usr/lib
                                    ENV LIB
                                    DOC "gmodule library to link with"
                                    NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GLIB_gobject_2_LIBRARY NAMES gobject-2.0
                                    PATHS ${GLIB2_DIR}/bin ${GLIB2_DIR}/win32/bin ${GLIB2_DIR}/lib ${GLIB2_DIR}/win32/lib /usr/local/lib /usr/lib
                                    ENV LIB
                                    DOC "gobject library to link with"
                                    NO_SYSTEM_ENVIRONMENT_PATH)

FIND_LIBRARY(GLIB_gthread_2_LIBRARY NAMES gthread-2.0
                                    PATHS ${GLIB2_DIR}/bin ${GLIB2_DIR}/win32/bin ${GLIB2_DIR}/lib ${GLIB2_DIR}/win32/lib /usr/local/lib /usr/lib
                                    ENV LIB
                                    DOC "gthread library to link with"
                                    NO_SYSTEM_ENVIRONMENT_PATH)

IF (GLIB_glib_2_INCLUDE_DIR AND GLIB_glibconfig_2_INCLUDE_DIR AND GLIB_glib_2_LIBRARY AND GLIB_gmodule_2_LIBRARY AND GLIB_gobject_2_LIBRARY AND GLIB_gthread_2_LIBRARY)
  SET(GLIB2_INCLUDE_DIR ${GLIB_glib_2_INCLUDE_DIR} ${GLIB_glibconfig_2_INCLUDE_DIR})
  list(REMOVE_DUPLICATES GLIB2_INCLUDE_DIR)
  SET(GLIB2_LIBRARIES ${GLIB_glib_2_LIBRARY} ${GLIB_gmodule_2_LIBRARY} ${GLIB_gobject_2_LIBRARY} ${GLIB_gthread_2_LIBRARY})
  list(REMOVE_DUPLICATES GLIB2_LIBRARIES)
  SET(GLIB2_FOUND TRUE)
ENDIF (GLIB_glib_2_INCLUDE_DIR AND GLIB_glibconfig_2_INCLUDE_DIR AND GLIB_glib_2_LIBRARY AND GLIB_gmodule_2_LIBRARY AND GLIB_gobject_2_LIBRARY AND GLIB_gthread_2_LIBRARY)
