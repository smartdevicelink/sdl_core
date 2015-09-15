include_directories (
  ./
)


set (SOURCES
	./sqlite3.c
)

add_library("sqlite3" ${SOURCES})
