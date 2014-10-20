include_directories(
  ./include/
  ../protocol_handler/include/
  ../connection_handler/include
  ../utils/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
  ${APR_INCLUDE_DIRECTORY}
)

set (SOURCES
  src/security_manager_impl.cc
  src/security_query.cc
  src/crypto_manager_impl.cc
  src/ssl_context_impl.cc
)

add_library(SecurityManager ${SOURCES})
target_link_libraries(SecurityManager crypto ssl ProtocolHandler jsoncpp ProtocolLibrary)
