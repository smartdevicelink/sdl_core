/*
 * Copyright (c) 2015, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "security_manager/crypto_manager_impl.h"

#ifdef __QNXNTO__
#include <openssl/ssl3.h>
#else
#include <openssl/tls1.h>
#endif

#ifdef __QNXNTO__
#define FORD_CIPHER SSL3_TXT_RSA_DES_192_CBC3_SHA
#else
// Used cipher from ford protocol requirement
#define FORD_CIPHER TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384
#endif

#define ALL_CIPHERS "ALL"

namespace {
const size_t updates_before_hour = 24;
}

namespace test {
namespace components {
namespace crypto_manager_test {

class CryptoManagerTest : public testing::Test {
 protected:
  void SetUp() OVERRIDE {
    crypto_manager = new security_manager::CryptoManagerImpl();
  }
  void TearDown() OVERRIDE { delete crypto_manager; }
  void InitSecurityManger() {
    const bool crypto_manager_initialization = crypto_manager->Init(
        security_manager::CLIENT, security_manager::TLSv1_2, "", ALL_CIPHERS,
        false, "/tmp/ca_cert.crt", updates_before_hour);
    ASSERT_TRUE(crypto_manager_initialization);
  }
  std::string GenerateCertificateString() {
    std::ifstream certificate_file("server/spt_credential.p12.enc");
    EXPECT_TRUE(certificate_file.good());

    const std::string cetrificate(
        (std::istreambuf_iterator<char>(certificate_file)),
        std::istreambuf_iterator<char>());
    EXPECT_FALSE(cetrificate.empty());
    return cetrificate;
  }

  security_manager::CryptoManager* crypto_manager;
};

TEST_F(CryptoManagerTest, UsingBeforeInit) {
  EXPECT_TRUE(crypto_manager->CreateSSLContext() == NULL);
  EXPECT_EQ(std::string("Initialization is not completed"),
            crypto_manager->LastError());
}

TEST_F(CryptoManagerTest, WrongInit) {
  // We have to cast (-1) to security_manager::Protocol Enum to be accepted by
  // crypto_manager->Init(...)
  security_manager::Protocol UNKNOWN =
      static_cast<security_manager::Protocol>(-1);

  std::ifstream file("spt_credential.p12.enc");
  std::stringstream ss;
  ss << file.rdbuf();

  // Unknown protocol version
  EXPECT_FALSE(crypto_manager->Init(security_manager::SERVER, UNKNOWN, ss.str(),
                                    FORD_CIPHER, false, "",
                                    updates_before_hour));

  EXPECT_FALSE(crypto_manager->LastError().empty());
  // Unexistent cipher value
  EXPECT_FALSE(crypto_manager->Init(
      security_manager::SERVER, security_manager::TLSv1_2, ss.str(),
      "INVALID_UNKNOWN_CIPHER", false, "", updates_before_hour));
  EXPECT_FALSE(crypto_manager->LastError().empty());
}

//#ifndef __QNXNTO__
TEST_F(CryptoManagerTest, CorrectInit) {
  bool is_initialyzed;
  std::string error_string;
  std::ifstream file("server/spt_credential.p12.enc");
  std::stringstream ss;
  ss << file.rdbuf();

  // Empty cert and key values for SERVER
  is_initialyzed = crypto_manager->Init(
      security_manager::SERVER, security_manager::TLSv1_2, ss.str(),
      FORD_CIPHER, false, "", updates_before_hour);
  EXPECT_TRUE(is_initialyzed);
  error_string = crypto_manager->LastError();
  EXPECT_TRUE(error_string.empty());

  // Recall init
  is_initialyzed =
      crypto_manager->Init(security_manager::CLIENT, security_manager::TLSv1_2,
                           "", FORD_CIPHER, false, "", updates_before_hour);
  error_string = crypto_manager->LastError();
  EXPECT_TRUE(error_string.empty());

  // Recall init with other protocols
  is_initialyzed =
      crypto_manager->Init(security_manager::CLIENT, security_manager::TLSv1_1,
                           "", FORD_CIPHER, false, "", updates_before_hour);
  error_string = crypto_manager->LastError();
  EXPECT_TRUE(error_string.empty());

  is_initialyzed =
      crypto_manager->Init(security_manager::CLIENT, security_manager::TLSv1,
                           "", FORD_CIPHER, false, "", updates_before_hour);
  error_string = crypto_manager->LastError();
  EXPECT_TRUE(error_string.empty());

  // Cipher value
  is_initialyzed = crypto_manager->Init(
      security_manager::SERVER, security_manager::TLSv1_2, ss.str(),
      ALL_CIPHERS, false, "", updates_before_hour);
  error_string = crypto_manager->LastError();
  EXPECT_TRUE(error_string.empty());
}
//#endif  // __QNX__

TEST_F(CryptoManagerTest, ReleaseSSLContext_Null) {
  EXPECT_NO_THROW(crypto_manager->ReleaseSSLContext(NULL));
}

TEST_F(CryptoManagerTest, CreateReleaseSSLContext) {
  EXPECT_TRUE(crypto_manager->Init(security_manager::CLIENT,
                                   security_manager::TLSv1_2, "", ALL_CIPHERS,
                                   false, "", updates_before_hour));
  security_manager::SSLContext* context = crypto_manager->CreateSSLContext();
  EXPECT_TRUE(context);
  EXPECT_NO_THROW(crypto_manager->ReleaseSSLContext(context));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated) {
  InitSecurityManger();

  const std::string cetrificate = GenerateCertificateString();
  ASSERT_FALSE(cetrificate.empty());

  EXPECT_TRUE(crypto_manager->OnCertificateUpdated(cetrificate));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_NotInitialized) {
  const std::string cetrificate = GenerateCertificateString();
  ASSERT_FALSE(cetrificate.empty());

  EXPECT_FALSE(crypto_manager->OnCertificateUpdated(cetrificate));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_NullString) {
  InitSecurityManger();
  EXPECT_FALSE(crypto_manager->OnCertificateUpdated(std::string()));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_MalformedSign) {
  InitSecurityManger();

  std::string cetrificate = GenerateCertificateString();
  ASSERT_FALSE(cetrificate.empty());
  // corrupt the middle symbol
  cetrificate[cetrificate.size() / 2] = '?';

  EXPECT_FALSE(crypto_manager->OnCertificateUpdated(cetrificate));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_WrongInitFolder) {
  const bool crypto_manager_initialization = crypto_manager->Init(
      ::security_manager::CLIENT, security_manager::TLSv1_2, "wrong_name",
      ALL_CIPHERS, true, "", updates_before_hour);
  ASSERT_TRUE(crypto_manager_initialization);

  std::string cetrificate = "wrong_data";
  ASSERT_FALSE(cetrificate.empty());

  EXPECT_FALSE(crypto_manager->OnCertificateUpdated(cetrificate));
}

}  // namespace crypto_manager_test
}  // namespace components
}  // namespace test
