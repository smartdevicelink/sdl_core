set(target vrmoduleplugin)
set(library_name ${CMAKE_SHARED_LIBRARY_PREFIX}${target}${CMAKE_SHARED_LIBRARY_SUFFIX})
set(install_destination ${CMAKE_BINARY_DIR}/bin/plugins)
set(copy_destination ${CMAKE_BINARY_DIR}/src/appMain/plugins)

if (ENABLE_GCOV)
  set(GCOV_FLAGS "-ftest-coverage -fprofile-arcs")
else()
  set(GCOV_FLAGS "")
endif()

set(CMAKE_CXX_FLAGS "-fPIC -std=gnu++0x -Wall -Werror ${GCOV_FLAGS}")

if (CMAKE_BUILD_TYPE)
  if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "")
  else ()
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb3 -DDEBUG")
  endif()
endif()

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ./include/
  ../include/
  ../functional_module/include/
  ../application_manager/include
  ../connection_handler/include/
  ../protocol/include/
  ../protocol_handler/include/
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
  ${GPB_INCLUDE_DIRECTORY}
)

set (SOURCES
    src/vr_module.cc
    src/vr_proxy.cc
    src/socket_channel.cc
    src/request_controller.cc
    src/hmi_event.cc
    src/mobile_event.cc
    src/convertor.cc
    src/commands/factory.cc
    src/commands/support_service.cc
    src/commands/activate_service.cc
    src/commands/on_default_service_chosen.cc
    src/commands/on_service_deactivated.cc
    src/commands/on_register_service.cc
    src/commands/on_unregister_service.cc
    src/commands/process_data.cc
)

set (LIBRARIES
    jsoncpp
    ApplicationManager
    FunctionalModule
    ConfigProfile
    Utils
    Net
    protobuf
    MobileProto
    HmiProto
)

generate_proto(${CMAKE_CURRENT_SOURCE_DIR}/interface/mobile.proto MobileProto)
generate_proto(${CMAKE_CURRENT_SOURCE_DIR}/interface/hmi.proto HmiProto)

link_directories(${LINK_DIRECTORIES} ${GPB_LIBS_DIRECTORY})
add_library(${target} SHARED ${SOURCES})
add_dependencies(${target} LibGPB)
target_link_libraries(${target} ${LIBRARIES} )

if(ENABLE_LOG)
  target_link_libraries(${target} log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

install(TARGETS ${target}
  DESTINATION ${install_destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

install(
  FILES ${FILES_FOR_COPY}
  DESTINATION ${install_destination}
)

add_subdirectory(net)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
