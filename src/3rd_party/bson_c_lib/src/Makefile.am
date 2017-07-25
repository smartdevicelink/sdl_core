SUBDIRS = emhashmap

AM_CFLAGS = -Wall
AM_LDFLAGS = -lemhashmap

includedir = /usr/local/include
include_HEADERS = bson_object.h bson_array.h bson_util.h

lib_LTLIBRARIES = libbson.la
libbson_la_SOURCES = bson_object.c bson_array.c bson_util.c
libbson_la_LIBADD = emhashmap/libemhashmap.la
