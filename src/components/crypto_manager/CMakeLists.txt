include_directories(include)

set (SOURCES
  src/crypto_manager_impl.cc
  src/ssl_context_impl.cc
)

add_library(CryptoManager ${SOURCES})
target_link_libraries(CryptoManager crypto ssl)

add_subdirectory(test)
