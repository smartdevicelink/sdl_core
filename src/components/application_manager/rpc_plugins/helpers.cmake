function(add_dynamic_library NAME)
  include(GNUInstallDirs)
  add_library(${NAME} MODULE)

  target_sources(${NAME} PRIVATE ${PLUGIN_SOURCES})

  target_link_libraries(${NAME}
                        PUBLIC rpc_base::rpc_base
                               utils::utils
                               smart_objects::smart_objects
                               interfaces::HMI_API
                               interfaces::MOBILE_API
                               interfaces::v4_protocol_v1_2_no_extra
                               connection_handler::connection_handler
                        PRIVATE application_manager::interface
                                components::interface)

  target_include_directories(${NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

  install(TARGETS ${NAME}
          DESTINATION "${SDL_PLUGINS_INSTALL_DIR}"
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
endfunction()

function(add_static_library NAME)
  add_library("${NAME}_static_lib" ${PLUGIN_SOURCES})

  target_link_libraries("${NAME}_static_lib"
                        PUBLIC rpc_base::rpc_base
                               utils::utils
                               smart_objects::smart_objects
                               interfaces::HMI_API
                               interfaces::MOBILE_API
                               interfaces::v4_protocol_v1_2_no_extra
                               connection_handler::connection_handler
                        PRIVATE application_manager::interface
                                components::interface)

  target_include_directories("${NAME}_static_lib"
                             PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
endfunction()
