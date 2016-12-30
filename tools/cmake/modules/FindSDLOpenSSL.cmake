#  - Try to find OpenSSL
#
#  OPENSSL_INCLUDE_DIRECTORY - the OpenSSL include directory
#  OPENSSL_LIBRARIES - the OpenSSL libraries

get_os(OS)
if(${OS} STREQUAL "posix")
  find_package(OpenSSL REQUIRED)
elseif(${OS} STREQUAL "win")
  get_arch(ARCH)
  if(${ARCH} STREQUAL "x86")
    set(OPENSSL_DIR ${CMAKE_SOURCE_DIR}/build/openssl_win_x86)
  elseif(${ARCH} STREQUAL "x64")
    set(OPENSSL_DIR ${CMAKE_SOURCE_DIR}/build/openssl_win_x64)
  endif()
  set(OPENSSL_INCLUDE_DIRECTORY ${OPENSSL_DIR}/include)
  set(OPENSSL_LIBRARIES ${OPENSSL_DIR}/lib/libeay32.lib ${OPENSSL_DIR}/lib/ssleay32.lib)
endif()
