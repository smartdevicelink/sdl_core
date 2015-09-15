

include_directories (
  ./
  ./lib
)


set (EXPAT_SOURCES
  ./lib/xmlparse.c
  ./lib/xmltok.c
  ./lib/xmlrole.c
)



add_library("expat" ${EXPAT_SOURCES})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#target_link_libraries(${PROJECT} ${LIBRARIES})
#link_library
#target_link_libraries("lib_msp_vr"  ${soxr-lsr})
endif()
