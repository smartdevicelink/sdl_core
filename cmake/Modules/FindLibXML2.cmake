find_path(LibXML2_ROOT_DIR NAMES include/libxml2/libxml/xpath.h)
find_library(LibXML2_LIBRARIES
    NAMES xml2
    HINTS ${LibXML2_ROOT_DIR}/lib
)
find_path(LibXML2_INCLUDE_DIR
    NAMES libxml/tree.h
    HINTS ${LibXML2_ROOT_DIR}/include/libxml2
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibXML2 DEFAULT_MSG
    LibXML2_LIBRARIES
    LibXML2_INCLUDE_DIR
)

mark_as_advanced(
    LibXML2_ROOT_DIR
    LibXML2_LIBRARIES
    LibXML2_INCLUDE_DIR
)