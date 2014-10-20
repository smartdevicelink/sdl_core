include_directories(
  include
  ${pugixml_SOURCE_DIR}/../src
  ${intergen_SOURCE_DIR}/utils/include
)

set (SOURCES
  src/model/api.cc
  src/model/builtin_type.cc
  src/model/builtin_type_registry.cc
  src/model/composite_type.cc
  src/model/constant.cc
  src/model/function.cc
  src/model/interface.cc
  src/model/model_filter.cc
  src/model/scope.cc
  src/model/type.cc
  src/model/type_registry.cc
)

set (HEADERS
  include/model/api.h
  include/model/builtin_type.h
  include/model/builtin_type_registry.h
  include/model/composite_type.h
  include/model/constant.h
  include/model/constants_creator.h
  include/model/function.h
  include/model/interface.h
  include/model/model_filter.h
  include/model/scope.h
  include/model/type.h
  include/model/type_registry.h
)

add_library(intergen_model ${HEADERS} ${SOURCES})
target_link_libraries(intergen_model intergen_utils pugixml)
