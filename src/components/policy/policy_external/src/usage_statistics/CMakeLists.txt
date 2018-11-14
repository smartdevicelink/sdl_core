set(target_name usage_statistics)

add_library(${target_name} STATIC)
add_library(${PROJECT_NAME}::${target_name} ALIAS ${target_name})

target_sources(${target_name}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/counter.cc"
)

target_link_libraries(${target_name}
  PUBLIC
    utils::utils
  PRIVATE
    components::interface
    EXPAT::EXPAT
)

target_include_directories(${target_name}
  PUBLIC
    "${${PROJECT_NAME}_INCLUDE_DIR}"
)
