set(INCLUDE_PATH "${CMAKE_SOURCE_PREFIX}/include")
set(LIB_PATH "${CMAKE_SOURCE_PREFIX}/lib")

find_path(BSON_INCLUDE_DIRECTORY bson_object.h bson_array.h bson_util.h
          PATHS "${INCLUDE_PATH}")

find_library(BSON_LIB
             NAMES bson
             PATHS ${LIB_PATH})

find_path(EMHASHMAP_INCLUDE_DIRECTORY emhashmap.h
          PATHS ${INCLUDE_PATH}
<<<<<<< 5fc2b1752f96e15ebd7433fab1d0d1a602273580
          PATH_SUFFIXES emhashmap)
=======
          PATH_SUFIXES emhashmap)
>>>>>>> Fix instalation of bson to avoid root access

find_library(EMHASHMAP_LIB
             NAMES emhashmap
             PATHS ${LIB_PATH})


include(FindPackageHandleStandardArgs)

<<<<<<< 5fc2b1752f96e15ebd7433fab1d0d1a602273580
find_package_handle_standard_args(BSON DEFAULT_MSG BSON_INCLUDE_DIRECTORY BSON_LIB
						   EMHASHMAP_INCLUDE_DIRECTORY EMHASHMAP_LIB)
=======
find_package_handle_standard_args(BSON DEFAULT_MSG BSON_INCLUDE_DIRECTORY BSON_INCLUDE_DIRECTORY)
>>>>>>> Fix instalation of bson to avoid root access

mark_as_advanced(BSON_INCLUDE_DIRECTORY BSON_LIB)
mark_as_advanced(EMHASHMAP_INCLUDE_DIRECTORY EMHASHMAP_LIB)
