# Copyright (c) 2018, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

project(smartDeviceLinkCore)

add_executable(${PROJECT_NAME})
add_executable(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/main.cc"
    "${CMAKE_CURRENT_LIST_DIR}/life_cycle_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/low_voltage_signals_handler.cc"
    "${CMAKE_CURRENT_LIST_DIR}/signal_handlers.cc"
)

find_package(Boost 1.66.0 COMPONENTS system filesystem)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    application_manager::application_manager
    media_manager::media_manager
    protocol_handler::protocol_handler
    $<$<BOOL:${ENABLE_SECURITY}>:security_manager::security_manager>
    transport_manager::transport_manager
    $<$<BOOL:${TELEMETRY_MONITOR}>:telemetry_monitor::telemetry_monitor>
    utils::utils
  PRIVATE
    components::interface
    Policy::usage_statistics
    Boost::system
    Boost::filesystem
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
      "${COMPONENTS_DIR}/protocol_handler/include"
      "${COMPONENTS_DIR}/application_manager/include"
      "${COMPONENTS_DIR}/formatters/include"
      "${COMPONENTS_DIR}/transport_manager/include"
      "${COMPONENTS_DIR}/security_manager/include"
      "${COMPONENTS_DIR}/security_manager/include"
      "${COMPONENTS_DIR}/config_profile/include"
      "${COMPONENTS_DIR}/utils/include"
      "${COMPONENTS_DIR}/connection_handler/include"
      "${COMPONENTS_DIR}/hmi_message_handler/include"
      "${COMPONENTS_DIR}/hmi_message_handler/src"
      "${COMPONENTS_DIR}/request_watchdog/include"
      "${COMPONENTS_DIR}/smart_objects/include"
      "${COMPONENTS_DIR}/media_manager/include"
      "${COMPONENTS_DIR}/telemetry_monitor/include"
      "${COMPONENTS_DIR}/functional_module/include"
      "${POLICY_PATH}/include"
      "${POLICY_GLOBAL_INCLUDE_PATH}/"
      "${COMPONENTS_DIR}/rpc_base/include"
      "${COMPONENTS_DIR}/resumption/include"
      "${CMAKE_BINARY_DIR}/src/components"
      "${JSONCPP_INCLUDE_DIRECTORY}"
      "${LOG4CXX_INCLUDE_DIRECTORY}"
      "${BSON_INCLUDE_DIRECTORY}"
      "${OPENSSL_INCLUDE_DIRECTORY}"
      "${default_media_inc}"
      "${MESSAGE_BROKER_INCLUDE_DIRECTORY}"
      "${BOOST_INCLUDE_DIR}"
      "${CMAKE_SOURCE_DIR}/src"
)


if(UNIX AND NOT QNXNTO)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      pthread
      dl
  )
elseif(QNXNTO)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      iconv
  )
endif()

if (BUILD_BT_SUPPORT)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE bluetooth
  )
endif()

if (BUILD_USB_SUPPORT)
  if(UNIX AND NOT QNXNTO)
    target_link_libraries(${PROJECT_NAME}
      PRIVATE libusb-1.0.so
    )
  endif()
endif()

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION "${CMAKE_INSTALL_BINDIR}"
  COMPONENT sdl_core
)

# Print/Generate SDL dependencies file only for QNX platform
if (QNXNTO)
  if (CMAKE_HOST_UNIX) # Available only for unix-based operation systems
    set(BUILD_IDS_FILE ${CMAKE_BINARY_DIR}/build_ids.txt)
    set(BUILD_IDS_SEARCH_PATH ${CMAKE_BINARY_DIR} ${CMAKE_FIND_ROOT_PATH})

    add_custom_command(
      TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/check_build_ids.sh
        ARGS ${BUILD_IDS_SEARCH_PATH} > ${BUILD_IDS_FILE}
      COMMAND cat ${BUILD_IDS_FILE}
      COMMENT "Checking build ids of SDL dependencies"
      VERBATIM
    )
  endif(CMAKE_HOST_UNIX)
endif(QNXNTO)

get_property(GIT_COMMIT GLOBAL PROPERTY SDL_VERSION_GIT_COMMIT)

set(SDL_IN_CONFIG_FILE "${CMAKE_CURRENT_LIST_DIR}/smartDeviceLink.ini")

if(QNXNTO)
  set(SDL_CONFIG_PATH "/${SDL_CONFIG_INSTALL_DIR}")
  set(SDL_CACERTIFICATE_PATH "/${SDL_CONFIG_INSTALL_DIR}")
  set(SDL_PLUGINS_PATH "/${SDL_PLUGINS_INSTALL_DIR}")
  set(SDL_LOG_PATH "/fs/storage/rwdata/logs")
  set(SDL_LOG_FILENAME "smartdevicelink.log")
  set(SDL_LOG_FILE "${SDL_LOG_PATH}/${SDL_LOG_FILENAME}")
  set(SDL_STORAGE_PATH "/fs/storage/rwdata/storage/sdl")
  set(SDL_TMPDIR_PATH "/fs/tmpfs")

  set(POLICY_QDB_DB_INITIALIZER "${CMAKE_CURRENT_LIST_DIR}/init_policy.sh")
  install(
    FILES ${POLICY_QDB_DB_INITIALIZER}
    DESTINATION "${SDL_CONFIG_INSTALL_DIR}"
    COMPONENT sdl_core
    PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
  )
else()
  set(SDL_CONFIG_PATH "${SDL_CONFIG_INSTALL_DIR}")
  set(SDL_CACERTIFICATE_PATH ".")
  set(SDL_PLUGINS_PATH "./")
  set(SDL_LOG_PATH ".")
  set(SDL_LOG_FILENAME "smartdevicelink.log")
  set(SDL_LOG_FILE ${SDL_LOG_FILENAME})
  set(SDL_STORAGE_PATH "storage")

  

  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties"
    "${CMAKE_CURRENT_BINARY_DIR}/log4cxx.properties"
  @ONLY
  NEWLINE_STYLE UNIX
  )

  list(APPEND SDL_LOG_CONFIG_LIST "${CMAKE_CURRENT_BINARY_DIR}/log4cxx.properties")

  set(BUILD_CONFIG_FILENAME "${CMAKE_CURRENT_BINARY_DIR}/build_config.txt")
  generate_build_config(${BUILD_CONFIG_FILENAME})

  list(APPEND SDL_LOG_CONFIG_LIST "${BUILD_CONFIG_FILENAME}")

  install(
    FILES
      "${CMAKE_CURRENT_SOURCE_DIR}/audio.8bit.wav"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT sdl_core
  )
endif()

set(SDL_OUT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/smartDeviceLink.ini")

install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/hmi_capabilities.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/sdl_preloaded_pt.json"
    ${SDL_LOG_CONFIG_LIST}
  DESTINATION ${SDL_CONFIG_INSTALL_DIR}
  COMPONENT sdl_core
)

configure_file(
  ${SDL_IN_CONFIG_FILE}
  ${SDL_OUT_CONFIG_FILE}
  @ONLY
  NEWLINE_STYLE UNIX
)

install(
  FILES
    ${SDL_OUT_CONFIG_FILE}
  DESTINATION ${SDL_CONFIG_INSTALL_DIR}
  COMPONENT sdl_core
)



