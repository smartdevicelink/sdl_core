/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifdef __QNXNTO__
#include <openssl/ssl3.h>
#else
#include <openssl/ssl.h>
#endif //__QNXNTO__
#include <limits>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "security_manager/crypto_manager_impl.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

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
  static void SetUpTestCase() {
    std::ifstream certificate_file("server/spt_credential.p12.enc");
    ASSERT_TRUE(certificate_file.good())
        << "Could not open certificate data file";

    const std::string certificate(
        (std::istreambuf_iterator<char>(certificate_file)),
        std::istreambuf_iterator<char>());
    EXPECT_FALSE(certificate.empty()) << "Certificate data file is empty";
    certificate_data_base64_ = certificate;
  }

  void SetUp() OVERRIDE {
    ASSERT_FALSE(certificate_data_base64_.empty());
    crypto_manager_ = utils::MakeShared<security_manager::CryptoManagerImpl>();
  }

  void InitSecurityManager() {
    const bool crypto_manager_initialization =
        crypto_manager_->Init(security_manager::CLIENT,
                             security_manager::TLSv1_2,
                             "",
                             ALL_CIPHERS,
                             false,
                             "/tmp/ca_cert.crt",
                             updates_before_hour);
    ASSERT_TRUE(crypto_manager_initialization);
  }

  utils::SharedPtr<security_manager::CryptoManagerImpl> crypto_manager_;
  static std::string certificate_data_base64_;
};
std::string CryptoManagerTest::certificate_data_base64_;

TEST_F(CryptoManagerTest, UsingBeforeInit) {
  EXPECT_TRUE(crypto_manager_->CreateSSLContext() == NULL);
  EXPECT_EQ(std::string("Initialization is not completed"),
            crypto_manager_->LastError());
}

TEST_F(CryptoManagerTest, WrongInit) {
  // We have to cast (-1) to security_manager::Protocol Enum to be accepted by
  // crypto_manager_->Init(...)
  security_manager::Protocol UNKNOWN =
      static_cast<security_manager::Protocol>(-1);
  // Unknown protocol version
  EXPECT_FALSE(crypto_manager_->Init(security_manager::SERVER,
                                    UNKNOWN,
                                    certificate_data_base64_,
                                    FORD_CIPHER,
                                    false,
                                    "",
                                    updates_before_hour));
  EXPECT_NE(std::string(), crypto_manager_->LastError());

  EXPECT_FALSE(crypto_manager_->LastError().empty());
  // Unexistent cipher value
  EXPECT_FALSE(crypto_manager_->Init(security_manager::SERVER,
                                    security_manager::TLSv1_2,
                                    certificate_data_base64_,
                                    "INVALID_UNKNOWN_CIPHER",
                                    false,
                                    "",
                                    updates_before_hour));

  EXPECT_NE(std::string(), crypto_manager_->LastError());
}

//  #ifndef __QNXNTO__
TEST_F(CryptoManagerTest, CorrectInit) {
  // Empty cert and key values for SERVER
  ASSERT_TRUE(crypto_manager_->Init(security_manager::SERVER,
                                   security_manager::TLSv1_2,
                                   certificate_data_base64_,
                                   FORD_CIPHER,
                                   false,
                                   "",
                                   updates_before_hour));

  // Recall init
  EXPECT_TRUE(crypto_manager_->Init(security_manager::CLIENT,
                                   security_manager::TLSv1_2,
                                   "",
                                   FORD_CIPHER,
                                   false,
                                   "",
                                   updates_before_hour));

  // Recall init with other protocols
  EXPECT_TRUE(crypto_manager_->Init(security_manager::CLIENT,
                                   security_manager::TLSv1_1,
                                   "",
                                   FORD_CIPHER,
                                   false,
                                   "",
                                   updates_before_hour));

  EXPECT_TRUE(crypto_manager_->Init(security_manager::CLIENT,
                                   security_manager::TLSv1,
                                   "",
                                   FORD_CIPHER,
                                   false,
                                   "",
                                   updates_before_hour));

  // Cipher value
  EXPECT_TRUE(crypto_manager_->Init(security_manager::SERVER,
                                   security_manager::TLSv1_2,
                                   certificate_data_base64_,
                                   ALL_CIPHERS,
                                   false,
                                   "",
                                   updates_before_hour));
}
// #endif  // __QNX__

TEST_F(CryptoManagerTest, ReleaseSSLContext_Null) {
  EXPECT_NO_THROW(crypto_manager_->ReleaseSSLContext(NULL));
}

TEST_F(CryptoManagerTest, CreateReleaseSSLContext) {
  ASSERT_TRUE(crypto_manager_->Init(security_manager::CLIENT,
                                   security_manager::TLSv1_2,
                                   "",
                                   ALL_CIPHERS,
                                   false,
                                   "",
                                   updates_before_hour));
  security_manager::SSLContext* context = crypto_manager_->CreateSSLContext();
  EXPECT_TRUE(context);
  EXPECT_NO_THROW(crypto_manager_->ReleaseSSLContext(context));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated) {
  InitSecurityManager();
  EXPECT_TRUE(crypto_manager_->OnCertificateUpdated(certificate_data_base64_));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_UpdateNotRequired) {
  size_t updates_before = 0;
  bool crypto_manager_initialization =
      crypto_manager_->Init(security_manager::CLIENT,
                           security_manager::TLSv1_2,
                           "",
                           ALL_CIPHERS,
                           false,
                           "/tmp/ca_cert.crt",
                           updates_before);
  ASSERT_TRUE(crypto_manager_initialization);
  EXPECT_FALSE(crypto_manager_->IsCertificateUpdateRequired());

  size_t max_updates_ = std::numeric_limits<size_t>::max();
  crypto_manager_initialization =
      crypto_manager_->Init(security_manager::CLIENT,
                           security_manager::TLSv1_2,
                           "",
                           ALL_CIPHERS,
                           false,
                           "/tmp/ca_cert.crt",
                           max_updates_);
  ASSERT_TRUE(crypto_manager_initialization);

  EXPECT_TRUE(crypto_manager_->IsCertificateUpdateRequired());
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_NotInitialized) {
  EXPECT_FALSE(crypto_manager_->OnCertificateUpdated(certificate_data_base64_));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_NullString) {
  InitSecurityManager();
  EXPECT_FALSE(crypto_manager_->OnCertificateUpdated(std::string()));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_MalformedSign) {
  InitSecurityManager();
  // Corrupt the middle symbol
  certificate_data_base64_[certificate_data_base64_.size() / 2] = '?';

  EXPECT_FALSE(crypto_manager_->OnCertificateUpdated(certificate_data_base64_));
}

TEST_F(CryptoManagerTest, OnCertificateUpdated_WrongInitFolder) {
  const bool crypto_manager_initialization =
      crypto_manager_->Init(::security_manager::CLIENT,
                           security_manager::TLSv1_2,
                           "wrong_name",
                           ALL_CIPHERS,
                           true,
                           "",
                           updates_before_hour);
  ASSERT_TRUE(crypto_manager_initialization);

  const std::string certificate = "wrong_data";
  ASSERT_FALSE(certificate.empty());

  EXPECT_FALSE(crypto_manager_->OnCertificateUpdated(certificate));
}

}  // namespace crypto_manager_test
}  // namespace components
}  // namespace test
