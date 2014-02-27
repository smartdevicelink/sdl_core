include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/crypto_manager/include
  include/
)

set (SOURCES
  ./src/crypto_manager_impl_test.cc
)

set (LIBRARIES
	crypto
	ssl
	gtest
	gtest_main
	CryptoManager
)

add_library("test_CryptoManager" ${SOURCES})
target_link_libraries("test_CryptoManager" ssl gtest CryptoManager crypto)

create_test("test_CryptoManagerTest" "${SOURCES}" "${LIBRARIES}")

# vim: set ts=2 sw=2 et:
