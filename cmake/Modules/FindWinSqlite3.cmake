#  - Try to find Sqlite3
#
#  SQLITE_INCLUDE_DIRECTORY - the Sqlite3 include directory
#  SQLITE_LIB_DIRECTORY - the Sqlite3 lib directory
if(NOT EXISTS $ENV{SDL_SQLITE_DIR}/sqlite3.lib)
  execute_process(
      COMMAND ${CMAKE_SOURCE_DIR}/ms/make_sqlite_lib.cmd
  )
endif()

set(SQLITE_INCLUDE_DIRECTORY $ENV{SDL_SQLITE_DIR})
set(SQLITE_LIB_DIRECTORY $ENV{SDL_SQLITE_DIR})