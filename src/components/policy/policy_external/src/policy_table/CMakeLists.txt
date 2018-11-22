set(target_name policy_table)

add_library(${target_name} STATIC)
add_library(${PROJECT_NAME}::${target_name} ALIAS ${target_name})

target_sources(${target_name}
               PRIVATE
               "${CMAKE_CURRENT_LIST_DIR}/enums.cc"
               "${CMAKE_CURRENT_LIST_DIR}/types.cc"
               "${CMAKE_CURRENT_LIST_DIR}/validation.cc")

target_link_libraries(${target_name}
                      PUBLIC utils::utils rpc_base::rpc_base
                      PRIVATE components::interface)

target_include_directories(${target_name}
                           PUBLIC "${${PROJECT_NAME}_INCLUDE_DIR}")
