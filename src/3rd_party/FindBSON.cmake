set(INCLUDE_PATH "${CMAKE_SOURCE_PREFIX}/include")
set(LIB_PATH "${CMAKE_SOURCE_PREFIX}/lib")

find_path(BSON_INCLUDE_DIRECTORY bson_object.h bson_array.h bson_util.h
          PATHS "${INCLUDE_PATH}")

find_library(BSON_LIB
             NAMES bson
             PATHS ${LIB_PATH})

find_path(EMHASHMAP_INCLUDE_DIRECTORY emhashmap.h
          PATHS ${INCLUDE_PATH}
          PATH_SUFFIXES emhashmap)

find_library(EMHASHMAP_LIB
             NAMES emhashmap
             PATHS ${LIB_PATH})


include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(BSON DEFAULT_MSG BSON_INCLUDE_DIRECTORY BSON_LIB
						   EMHASHMAP_INCLUDE_DIRECTORY EMHASHMAP_LIB)

mark_as_advanced(BSON_INCLUDE_DIRECTORY BSON_LIB)
mark_as_advanced(EMHASHMAP_INCLUDE_DIRECTORY EMHASHMAP_LIB)
