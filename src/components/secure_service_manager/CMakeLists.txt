include_directories(
  ./include/
  ../protocol_handler/include/
  ../transport_manager/include/
  ../connection_handler/include
  ../utils/include/
)

set (SOURCES
  src/secure_service_manager.cc
  src/secure_service_query.cc
  src/crypto_manager_impl.cc
  src/ssl_context_impl.cc
)

add_library(CryptoManager ${SOURCES})
target_link_libraries(CryptoManager crypto ssl ProtocolHandler)
