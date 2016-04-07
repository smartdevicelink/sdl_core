#  - Try to find OpenSSL
#
#  OPENSSL_INCLUDE_DIRECTORY - the OpenSSL include directory
#  OPENSSL_LIB_DIRECTORY - the OpenSSL lib directory
if(QT_PORT)
  set(OPENSSL_DIR ${CMAKE_SOURCE_DIR}/build/openssl_win_x86)
else()
  set(OPENSSL_DIR ${CMAKE_SOURCE_DIR}/build/openssl_win_x64)
endif()

set(OPENSSL_INCLUDE_DIRECTORY ${OPENSSL_DIR}/include)
set(OPENSSL_LIB_DIRECTORY ${OPENSSL_DIR}/lib)
