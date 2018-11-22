project(Policy)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} SHARED)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(${PROJECT_NAME}_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

target_sources(${PROJECT_NAME}
               PRIVATE
               "${CMAKE_CURRENT_LIST_DIR}/src/access_remote_impl.cc"
               "${CMAKE_CURRENT_LIST_DIR}/src/cache_manager.cc"
               "${CMAKE_CURRENT_LIST_DIR}/src/policy_helper.cc"
               "${CMAKE_CURRENT_LIST_DIR}/src/policy_manager_impl.cc"
               "${CMAKE_CURRENT_LIST_DIR}/src/policy_table.cc"
               "${CMAKE_CURRENT_LIST_DIR}/src/sql_pt_queries.cc"
               "${CMAKE_CURRENT_LIST_DIR}/src/sql_pt_representation.cc"
               "${CMAKE_CURRENT_LIST_DIR}/src/status.cc"
               "${CMAKE_CURRENT_LIST_DIR}/src/update_status_manager.cc")

target_link_libraries(${PROJECT_NAME}
                      PUBLIC jsoncpp::jsoncpp
                             utils::utils
                             utils::dbms
                             config_profile::config_profile
                             Policy::policy_table
                      PRIVATE components::interface)

if(ENABLE_HMI_PTU_DECRYPTION)
  target_compile_definitions(${PROJECT_NAME} PUBLIC USE_HMI_PTU_DECRYPTION)
endif()

target_include_directories(${PROJECT_NAME}
                           PUBLIC "${${PROJECT_NAME}_INCLUDE_DIR}")

install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        COMPONENT
        sdl_core
        PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE)

add_subdirectory(src/policy_table)
add_subdirectory(src/usage_statistics)
