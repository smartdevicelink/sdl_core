project(sqlite)

# Adding static libraries
add_library(sqlite_static STATIC sqlite3.c)
