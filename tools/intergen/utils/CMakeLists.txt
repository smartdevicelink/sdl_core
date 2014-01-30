include_directories(
  include
  ${pugixml_SOURCE_DIR}/../src
)

set (HEADERS
  include/utils/common_types.h
  include/utils/macro.h
  include/utils/safeformat.h
  include/utils/stl_utils.h
  include/utils/string_utils.h
  include/utils/xml_utils.h
)

set (SOURCES
  src/utils/common_types.cc
  src/utils/safeformat.cc
  src/utils/xml_utils.cc
)

add_library(intergen_utils ${HEADERS} ${SOURCES})
target_link_libraries(intergen_utils pugixml)
