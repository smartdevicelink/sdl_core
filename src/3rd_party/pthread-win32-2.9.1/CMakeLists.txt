
include_directories(
.
)

set (SOURCES
  pthread.c
)

set (HEADERS
  ./implement.h
  ./pthread.h
  ./sched.h
  ./semaphore.h
)

add_definitions(-DOS_WINCE)
add_definitions(-DHAVE_PTW32_CONFIG_H)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)
add_definitions(-DWINCE)
add_definitions(-D_WIN32)

add_library(pthreadVC2 ${HEADERS} ${SOURCES})

