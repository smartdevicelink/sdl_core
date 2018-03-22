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
#endif  //__QNXNTO__
#include <limits>
#include <fstream>
#include <sstream>

#include "utils/make_shared.h"
#include "gtest/gtest.h"
#include "security_manager/crypto_manager_impl.h"
#include "security_manager/mock_security_manager_settings.h"

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;

namespace {
const size_t kUpdatesBeforeHour = 24;
const std::string kAllCiphers = "ALL";
const std::string kCaCertPath = "";
const uint32_t kServiceNumber = 2u;
const size_t kMaxSizeVector = 1u;
#ifdef __QNXNTO__
const std::string kFordCipher = SSL3_TXT_RSA_DES_192_CBC3_SHA;
#else
// Used cipher from ford protocol requirement
const std::string kFordCipher = TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384;
#endif
}

namespace test {
namespace components {
namespace crypto_manager_test {

using security_manager::CryptoManagerImpl;

class CryptoManagerTest : public testing::Test {
 protected:
  typedef NiceMock<security_manager_test::MockCryptoManagerSettings>
      MockCryptoManagerSettings;
  static void SetUpTestCase() {
    std::ifstream certificate_file("server/spt_credential.pem");
    ASSERT_TRUE(certificate_file.is_open())
        << "Could not open certificate data file";

    const std::string certificate(
        (std::istreambuf_iterator<char>(certificate_file)),
        std::istreambuf_iterator<char>());
    ASSERT_FALSE(certificate.empty()) << "Certificate data file is empty";
    certificate_data_base64_ = certificate;
  }

  void SetUp() OVERRIDE {
    ASSERT_FALSE(certificate_data_base64_.empty());
    mock_security_manager_settings_ =
        utils::MakeShared<MockCryptoManagerSettings>();
    crypto_manager_ =
        utils::MakeShared<CryptoManagerImpl>(mock_security_manager_settings_);
    forced_protected_services_.reserve(kMaxSizeVector);
    forced_unprotected_services_.reserve(kMaxSizeVector);
  }

  void InitSecurityManager() {
    SetInitialValues(
        security_manager::CLIENT, security_manager::TLSv1_2, kAllCiphers);
    const bool crypto_manager_initialization = crypto_manager_->Init();
    ASSERT_TRUE(crypto_manager_initialization);
  }

  void SetInitialValues(security_manager::Mode mode,
                        security_manager::Protocol protocol,
                        const std::string& cipher) {
    ON_CALL(*mock_security_manager_settings_, force_unprotected_service())
        .WillByDefault(ReturnRef(forced_unprotected_services_));
    ON_CALL(*mock_security_manager_settings_, force_protected_service())
        .WillByDefault(ReturnRef(forced_protected_services_));
    ON_CALL(*mock_security_manager_settings_, security_manager_mode())
        .WillByDefault(Return(mode));
    ON_CALL(*mock_security_manager_settings_, security_manager_protocol_name())
        .WillByDefault(Return(protocol));
    ON_CALL(*mock_security_manager_settings_, certificate_data())
        .WillByDefault(ReturnRef(certificate_data_base64_));
    ON_CALL(*mock_security_manager_settings_, ciphers_list())
        .WillByDefault(ReturnRef(cipher));
    ON_CALL(*mock_security_manager_settings_, ca_cert_path())
        .WillByDefault(ReturnRef(kCaCertPath));
    ON_CALL(*mock_security_manager_settings_, verify_peer())
        .WillByDefault(Return(false));
  }

  utils::SharedPtr<CryptoManagerImpl> crypto_manager_;
  utils::SharedPtr<MockCryptoManagerSettings> mock_security_manager_settings_;
  static std::string certificate_data_base64_;
  std::vector<int> forced_protected_services_;
  std::vector<int> forced_unprotected_services_;
};

std::string CryptoManagerTest::certificate_data_base64_;

TEST_F(CryptoManagerTest, UsingBeforeInit) {
  EXPECT_TRUE(crypto_manager_->CreateSSLContext() == NULL);
  EXPECT_EQ(std::string("Initialization is not completed"),
            crypto_manager_->LastError());
}

TEST_F(CryptoManagerTest, WrongInit) {
  forced_protected_services_.push_back(kServiceNumber);
  forced_unprotected_services_.push_back(kServiceNumber);
  EXPECT_CALL(*mock_security_manager_settings_, security_manager_mode())
      .WillOnce(Return(security_manager::SERVER));
  EXPECT_CALL(*mock_security_manager_settings_, force_unprotected_service())
      .WillOnce(ReturnRef(forced_unprotected_services_));
  EXPECT_CALL(*mock_security_manager_settings_, force_protected_service())
      .WillOnce(ReturnRef(forced_protected_services_));
  EXPECT_FALSE(crypto_manager_->Init());
  forced_protected_services_.pop_back();
  forced_unprotected_services_.pop_back();
  EXPECT_NE(std::string(), crypto_manager_->LastError());
  // Unexistent cipher value
  const std::string invalid_cipher = "INVALID_UNKNOWN_CIPHER";
  EXPECT_CALL(*mock_security_manager_settings_, security_manager_mode())
      .WillOnce(Return(security_manager::SERVER));
  EXPECT_CALL(*mock_security_manager_settings_, force_unprotected_service())
      .WillOnce(ReturnRef(forced_unprotected_services_));
  EXPECT_CALL(*mock_security_manager_settings_, force_protected_service())
      .WillOnce(ReturnRef(forced_protected_services_));
  EXPECT_CALL(*mock_security_manager_settings_,
              security_manager_protocol_name())
      .WillOnce(Return(security_manager::TLSv1_2));
  EXPECT_CALL(*mock_security_manager_settings_, certificate_data())
      .WillOnce(ReturnRef(certificate_data_base64_));
  EXPECT_CALL(*mock_security_manager_settings_, ciphers_list())
      .WillRepeatedly(ReturnRef(invalid_cipher));
  EXPECT_FALSE(crypto_manager_->Init());
  EXPECT_NE(std::string(), crypto_manager_->LastError());
}

//  #ifndef __QNXNTO__
TEST_F(CryptoManagerTest, CorrectInit) {
  // Empty cert and key values for SERVER
  SetInitialValues(
      security_manager::SERVER, security_manager::TLSv1_2, kFordCipher);
  EXPECT_TRUE(crypto_manager_->Init());

  // Recall init
  SetInitialValues(
      security_manager::CLIENT, security_manager::TLSv1_2, kFordCipher);
  EXPECT_TRUE(crypto_manager_->Init());

  // Recall init with other protocols
  SetInitialValues(
      security_manager::CLIENT, security_manager::TLSv1_2, kFordCipher);
  EXPECT_TRUE(crypto_manager_->Init());

  SetInitialValues(
      security_manager::CLIENT, security_manager::TLSv1_1, kFordCipher);
  EXPECT_TRUE(crypto_manager_->Init());

  SetInitialValues(
      security_manager::CLIENT, security_manager::DTLSv1, kFordCipher);
  EXPECT_TRUE(crypto_manager_->Init());

  // Cipher value
  SetInitialValues(
      security_manager::SERVER, security_manager::TLSv1_2, kAllCiphers);
  EXPECT_TRUE(crypto_manager_->Init());

  SetInitialValues(
      security_manager::SERVER, security_manager::DTLSv1, kAllCiphers);
  EXPECT_TRUE(crypto_manager_->Init());
}
// #endif  // __QNX__

TEST_F(CryptoManagerTest, ReleaseSSLContext_Null) {
  EXPECT_NO_THROW(crypto_manager_->ReleaseSSLContext(NULL));
}

TEST_F(CryptoManagerTest, CreateReleaseSSLContext) {
  const size_t max_payload_size = 1000u;
  SetInitialValues(
      security_manager::CLIENT, security_manager::TLSv1_2, kAllCiphers);
  EXPECT_TRUE(crypto_manager_->Init());
  EXPECT_CALL(*mock_security_manager_settings_, security_manager_mode())
      .Times(2)
      .WillRepeatedly(Return(security_manager::CLIENT));
  EXPECT_CALL(*mock_security_manager_settings_, maximum_payload_size())
      .Times(1)
      .WillRepeatedly(Return(max_payload_size));
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
  SetInitialValues(
      security_manager::CLIENT, security_manager::TLSv1_2, kAllCiphers);
  ASSERT_TRUE(crypto_manager_->Init());

  EXPECT_CALL(*mock_security_manager_settings_, update_before_hours())
      .WillOnce(Return(updates_before));

  EXPECT_FALSE(crypto_manager_->IsCertificateUpdateRequired());

  size_t max_updates_ = std::numeric_limits<size_t>::max();
  SetInitialValues(
      security_manager::CLIENT, security_manager::TLSv1_2, kAllCiphers);
  EXPECT_CALL(*mock_security_manager_settings_, update_before_hours())
      .WillOnce(Return(max_updates_));
  ASSERT_TRUE(crypto_manager_->Init());

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
  SetInitialValues(
      security_manager::CLIENT, security_manager::TLSv1_2, kAllCiphers);
  ASSERT_TRUE(crypto_manager_->Init());

  const std::string certificate = "wrong_data";
  ASSERT_FALSE(certificate.empty());

  EXPECT_FALSE(crypto_manager_->OnCertificateUpdated(certificate));
}

}  // namespace crypto_manager_test
}  // namespace components
}  // namespace test
