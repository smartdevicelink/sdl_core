include_directories(../include)

set(SOURCES
    test_crypto_manager.cc
   )

add_executable(test ${SOURCES})

target_link_libraries(test crypto ssl CryptoManager)
