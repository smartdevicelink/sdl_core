#  - Try to find Sqlite3
#
#  SQLITE3_INCLUDE_DIRECTORY - the Sqlite3 include directory
#  SQLITE3_LIBRARIES - the Sqlite3 libraries

get_os(OS)
if(${OS} STREQUAL "posix")
  find_package(Sqlite3 REQUIRED)
elseif(${OS} STREQUAL "win")
  get_sdk(SDK)
  if(NOT SDK OR NOT ${SDK} STREQUAL "qt")
    set(SQLITE3_INCLUDE_DIRECTORY $ENV{SDL_SQLITE_DIR})
    set(SQLITE3_LIBRARIES $ENV{SDL_SQLITE_DIR}/sqlite3.lib)
  endif()
endif()
