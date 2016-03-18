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

#include "gtest/gtest.h"
#include <fstream>
#include <sstream>
#include <string>
#include <openssl/ssl.h>

#include "security_manager/crypto_manager.h"
#include "security_manager/crypto_manager_impl.h"
#include "security_manager/ssl_context.h"
#include "utils/custom_string.h"
#include "security_manager/mock_security_manager_settings.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;

namespace {
const size_t kUpdatesBeforeHour = 24;
const std::string kCaPath = "";
const uint8_t* kServerBuf;
const uint8_t* kClientBuf;
const std::string kAllCiphers = "ALL";
size_t server_buf_len;
size_t client_buf_len;
#ifdef __QNXNTO__
const std::string kFordCipher = SSL3_TXT_RSA_DES_192_CBC3_SHA;
#else
// Used cipher from ford protocol requirement
const std::string kFordCipher = TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384;
#endif
}  // namespace

namespace test {
namespace components {
namespace ssl_context_test {
namespace custom_str = utils::custom_string;

struct ProtocolAndCipher {
  security_manager::Protocol server_protocol;
  security_manager::Protocol client_protocol;
  std::string server_ciphers_list;
  std::string client_ciphers_list;

  ProtocolAndCipher(security_manager::Protocol s_protocol,
                    security_manager::Protocol c_protocol,
                    std::string s_ciphers_list,
                    std::string c_ciphers_list)
      : server_protocol(s_protocol)
      , client_protocol(c_protocol)
      , server_ciphers_list(s_ciphers_list)
      , client_ciphers_list(c_ciphers_list) {}
};

class SSLTest : public testing::Test {
 protected:
  static void SetUpTestCase() {
    std::ifstream certificate_file("server/spt_credential_unsigned.p12");
    std::stringstream certificate;
    if (certificate_file.is_open()) {
      certificate << certificate_file.rdbuf();
    }
    certificate_file.close();
    certificate_data_base64_ = certificate.str();
    ASSERT_FALSE(certificate_data_base64_.empty()) << "Certificate data file is empty";
  }

  virtual void SetUp() OVERRIDE {
    mock_crypto_manager_settings_ = utils::MakeShared<
        NiceMock<security_manager_test::MockCryptoManagerSettings>>();
    utils::SharedPtr<security_manager::CryptoManagerSettings> crypto_set(
        mock_crypto_manager_settings_);
    crypto_manager_ = new security_manager::CryptoManagerImpl(crypto_set);

    EXPECT_CALL(*mock_crypto_manager_settings_, security_manager_mode())
        .WillOnce(Return(security_manager::SERVER));
    EXPECT_CALL(*mock_crypto_manager_settings_,
                security_manager_protocol_name())
        .WillOnce(Return(security_manager::TLSv1_2));
    EXPECT_CALL(*mock_crypto_manager_settings_, certificate_data())
        .WillOnce(ReturnRef(certificate_data_base64_));
    EXPECT_CALL(*mock_crypto_manager_settings_, ciphers_list())
        .WillRepeatedly(ReturnRef(kAllCiphers));
    EXPECT_CALL(*mock_crypto_manager_settings_, ca_cert_path())
        .WillRepeatedly(ReturnRef(kCaPath));
    EXPECT_CALL(*mock_crypto_manager_settings_, verify_peer())
        .WillOnce(Return(false));
    const bool crypto_manager_initialization = crypto_manager_->Init();
    EXPECT_TRUE(crypto_manager_initialization);

    mock_client_manager_settings_ = utils::MakeShared<
        NiceMock<security_manager_test::MockCryptoManagerSettings>>();
    utils::SharedPtr<security_manager::CryptoManagerSettings> client_crypto(
        mock_client_manager_settings_);
    client_manager_ = new security_manager::CryptoManagerImpl(client_crypto);

    EXPECT_CALL(*mock_client_manager_settings_, security_manager_mode())
        .WillOnce(Return(security_manager::CLIENT));
    EXPECT_CALL(*mock_client_manager_settings_,
                security_manager_protocol_name())
        .WillOnce(Return(security_manager::TLSv1_2));
    EXPECT_CALL(*mock_client_manager_settings_, certificate_data())
        .WillOnce(ReturnRef(certificate_data_base64_));
    EXPECT_CALL(*mock_client_manager_settings_, ciphers_list())
        .WillRepeatedly(ReturnRef(kAllCiphers));
    EXPECT_CALL(*mock_client_manager_settings_, ca_cert_path())
        .Times(2)
        .WillRepeatedly(ReturnRef(kCaPath));
    EXPECT_CALL(*mock_client_manager_settings_, verify_peer())
        .WillOnce(Return(false));
    const bool client_manager_initialization = client_manager_->Init();
    EXPECT_TRUE(client_manager_initialization);

    ON_CALL(*mock_crypto_manager_settings_, maximum_payload_size())
        .WillByDefault(Return(kMaximumPayloadSize));
    ON_CALL(*mock_client_manager_settings_, maximum_payload_size())
        .WillByDefault(Return(kMaximumPayloadSize));
    EXPECT_CALL(*mock_crypto_manager_settings_, security_manager_mode())
        .WillRepeatedly(Return(security_manager::SERVER));
    server_ctx = crypto_manager_->CreateSSLContext();
    EXPECT_CALL(*mock_client_manager_settings_, security_manager_mode())
        .Times(2)
        .WillRepeatedly(Return(security_manager::CLIENT));
    client_ctx = client_manager_->CreateSSLContext();

    security_manager::SSLContext::HandshakeContext ctx;
    ctx.make_context(custom_str::CustomString("SPT"),
                     custom_str::CustomString("client"));
    server_ctx->SetHandshakeContext(ctx);

    ctx.expected_cn = "server";
    client_ctx->SetHandshakeContext(ctx);

    kServerBuf = NULL;
    kClientBuf = NULL;
    server_buf_len = 0u;
    client_buf_len = 0u;
  }

  virtual void TearDown() OVERRIDE {
    crypto_manager_->ReleaseSSLContext(server_ctx);
    client_manager_->ReleaseSSLContext(client_ctx);

    delete crypto_manager_;
    delete client_manager_;
  }

  const size_t kMaximumPayloadSize = 1000u;
  security_manager::CryptoManager* crypto_manager_;
  security_manager::CryptoManager* client_manager_;
  utils::SharedPtr<NiceMock<security_manager_test::MockCryptoManagerSettings>>
      mock_crypto_manager_settings_;
  utils::SharedPtr<NiceMock<security_manager_test::MockCryptoManagerSettings>>
      mock_client_manager_settings_;
  security_manager::SSLContext* server_ctx;
  security_manager::SSLContext* client_ctx;

  static std::string certificate_data_base64_;
};
std::string SSLTest::certificate_data_base64_;

// StartHandshake() fails when client and server protocols are not TLSv1_2
class SSLTestParam : public testing::TestWithParam<ProtocolAndCipher> {
 protected:
  virtual void SetUp() OVERRIDE {
    std::ifstream certificate_file("server/spt_credential.p12");
    ASSERT_TRUE(certificate_file.is_open())
        << "Could not open certificate data file";

    const std::string certificate(
        (std::istreambuf_iterator<char>(certificate_file)),
        std::istreambuf_iterator<char>());
    certificate_file.close();
    ASSERT_FALSE(certificate.empty()) << "Certificate data file is empty";
    certificate_data_base64_ = certificate;

    mock_crypto_manager_settings_ = utils::MakeShared<
        NiceMock<security_manager_test::MockCryptoManagerSettings>>();
    utils::SharedPtr<security_manager::CryptoManagerSettings> server_crypto(
                mock_crypto_manager_settings_);
    crypto_manager = new security_manager::CryptoManagerImpl(server_crypto);

    SetServerInitialValues(GetParam().server_protocol,
                           GetParam().server_ciphers_list);

    const bool crypto_manager_initialization = crypto_manager->Init();
    EXPECT_TRUE(crypto_manager_initialization);

    mock_client_manager_settings_ = utils::MakeShared<
        NiceMock<security_manager_test::MockCryptoManagerSettings>>();

    utils::SharedPtr<security_manager::CryptoManagerSettings> client_crypto(
                mock_client_manager_settings_);
    client_manager = new security_manager::CryptoManagerImpl(client_crypto);

    SetClientInitialValues(GetParam().client_protocol,
                           GetParam().client_ciphers_list);

    const bool client_manager_initialization = client_manager->Init();
    EXPECT_TRUE(client_manager_initialization);

    server_ctx = crypto_manager->CreateSSLContext();
    client_ctx = client_manager->CreateSSLContext();

    security_manager::SSLContext::HandshakeContext ctx;
    ctx.make_context(custom_str::CustomString("SPT"),
                     custom_str::CustomString("client"));
    server_ctx->SetHandshakeContext(ctx);

    ctx.expected_cn = "server";
    client_ctx->SetHandshakeContext(ctx);

    kServerBuf = NULL;
    kClientBuf = NULL;
    server_buf_len = 0u;
    client_buf_len = 0u;
  }

  virtual void TearDown() OVERRIDE {
    crypto_manager->ReleaseSSLContext(server_ctx);
    client_manager->ReleaseSSLContext(client_ctx);

    delete crypto_manager;
    delete client_manager;
  }

  void SetServerInitialValues(security_manager::Protocol protocol,
                              const std::string& server_ciphers_list) {
    ON_CALL(*mock_crypto_manager_settings_, security_manager_mode())
        .WillByDefault(Return(security_manager::SERVER));
    ON_CALL(*mock_crypto_manager_settings_, security_manager_protocol_name())
        .WillByDefault(Return(protocol));
    ON_CALL(*mock_crypto_manager_settings_, certificate_data())
        .WillByDefault(ReturnRef(certificate_data_base64_));
    ON_CALL(*mock_crypto_manager_settings_, ciphers_list())
        .WillByDefault(ReturnRef(server_ciphers_list));
    ON_CALL(*mock_crypto_manager_settings_, ca_cert_path())
        .WillByDefault(ReturnRef(kCaPath));
    ON_CALL(*mock_crypto_manager_settings_, verify_peer())
        .WillByDefault(Return(false));
  }
  void SetClientInitialValues(security_manager::Protocol protocol,
                              const std::string& client_ciphers_list) {
    ON_CALL(*mock_client_manager_settings_, security_manager_mode())
        .WillByDefault(Return(security_manager::CLIENT));
    ON_CALL(*mock_client_manager_settings_, security_manager_protocol_name())
        .WillByDefault(Return(protocol));
    ON_CALL(*mock_client_manager_settings_, certificate_data())
        .WillByDefault(ReturnRef(certificate_data_base64_));
    ON_CALL(*mock_client_manager_settings_, ciphers_list())
        .WillByDefault(ReturnRef(client_ciphers_list));
    ON_CALL(*mock_client_manager_settings_, ca_cert_path())
        .WillByDefault(ReturnRef(kCaPath));
    ON_CALL(*mock_client_manager_settings_, verify_peer())
        .WillByDefault(Return(false));
  }

  utils::SharedPtr<NiceMock<security_manager_test::MockCryptoManagerSettings>>
      mock_crypto_manager_settings_;
  utils::SharedPtr<NiceMock<security_manager_test::MockCryptoManagerSettings>>
      mock_client_manager_settings_;
  security_manager::CryptoManager* crypto_manager;
  security_manager::CryptoManager* client_manager;
  security_manager::SSLContext* server_ctx;
  security_manager::SSLContext* client_ctx;
  std::string certificate_data_base64_;
};

class SSLTestForTLS1_2 : public SSLTestParam {};

// This case fails starting because we can handshake only with TLSv1_2 protocol.
INSTANTIATE_TEST_CASE_P(
    CorrectProtocolAndCiphers,
    SSLTestParam,
    ::testing::Values(ProtocolAndCipher(security_manager::TLSv1,
                                        security_manager::TLSv1,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::TLSv1_1,
                                        security_manager::TLSv1_1,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::SSLv3,
                                        security_manager::SSLv3,
                                        kFordCipher,
                                        kFordCipher)));

INSTANTIATE_TEST_CASE_P(
    IncorrectProtocolAndCiphers,
    SSLTestParam,
    ::testing::Values(ProtocolAndCipher(security_manager::TLSv1,
                                        security_manager::TLSv1_1,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::TLSv1,
                                        security_manager::SSLv3,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::TLSv1_1,
                                        security_manager::TLSv1,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::TLSv1_1,
                                        security_manager::SSLv3,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::TLSv1_2,
                                        security_manager::TLSv1,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::TLSv1_2,
                                        security_manager::TLSv1_1,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::TLSv1_2,
                                        security_manager::SSLv3,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::SSLv3,
                                        security_manager::TLSv1,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::SSLv3,
                                        security_manager::TLSv1_1,
                                        kFordCipher,
                                        kFordCipher)));

TEST_F(SSLTest, OnTSL2Protocol_BrokenHandshake) {
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
            client_ctx->StartHandshake(&kClientBuf, &client_buf_len));
  ASSERT_FALSE(NULL == kClientBuf);
  ASSERT_LT(0u, client_buf_len);
  // Broke 3 bytes for get abnormal fail of handshake
  const_cast<uint8_t*>(kClientBuf)[0] ^= 0xFF;
  const_cast<uint8_t*>(kClientBuf)[client_buf_len / 2] ^= 0xFF;
  const_cast<uint8_t*>(kClientBuf)[client_buf_len - 1] ^= 0xFF;
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_AbnormalFail,
            server_ctx->DoHandshakeStep(
                kClientBuf, client_buf_len, &kServerBuf, &server_buf_len));
  EXPECT_EQ("Initialization is not completed", server_ctx->LastError());
  EXPECT_EQ("Initialization is not completed", client_ctx->LastError());
}

// TODO {AKozoriz} : Unexpected uncomplited init of SSL component.
// In this and next tests.
// Must be fixed after merge to develop.
TEST_F(SSLTest, DISABLED_OnTSL2Protocol_Positive) {
  ASSERT_EQ(client_ctx->StartHandshake(&kClientBuf, &client_buf_len),
            security_manager::SSLContext::Handshake_Result_Success);
  ASSERT_FALSE(NULL == kClientBuf);
  ASSERT_LT(0u, client_buf_len);
  EXPECT_TRUE(server_ctx->IsInitCompleted());

  while (true) {
    const security_manager::SSLContext::HandshakeResult server_result =
        server_ctx->DoHandshakeStep(
            kClientBuf, client_buf_len, &kServerBuf, &server_buf_len);
    ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
              server_result);
    ASSERT_FALSE(NULL == kServerBuf);
    ASSERT_LT(0u, server_buf_len);

    const security_manager::SSLContext::HandshakeResult client_result =
        client_ctx->DoHandshakeStep(
            kServerBuf, server_buf_len, &kClientBuf, &client_buf_len);
    ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
              client_result);
    if (server_ctx->IsInitCompleted()) {
      break;
    }

    ASSERT_FALSE(NULL == kClientBuf);
    ASSERT_LT(0u, client_buf_len);
  }
  // Expect empty buffers after init complete
  ASSERT_TRUE(NULL == kClientBuf);
  ASSERT_EQ(0u, client_buf_len);
  // expect both side initialization complete
  EXPECT_TRUE(client_ctx->IsInitCompleted());
  EXPECT_TRUE(server_ctx->IsInitCompleted());

  // Encrypt text on client side
  const uint8_t* text = reinterpret_cast<const uint8_t*>("abra");
  const uint8_t* encrypted_text = 0;
  size_t text_len = 4;
  size_t encrypted_text_len;
  EXPECT_TRUE(client_ctx->Encrypt(
      text, text_len, &encrypted_text, &encrypted_text_len));

  ASSERT_NE(reinterpret_cast<void*>(NULL), encrypted_text);
  ASSERT_LT(0u, encrypted_text_len);

  // Decrypt text on server side
  EXPECT_TRUE(server_ctx->Decrypt(
      encrypted_text, encrypted_text_len, &text, &text_len));
  ASSERT_NE(reinterpret_cast<void*>(NULL), text);
  ASSERT_LT(0u, text_len);

  ASSERT_EQ(strncmp(reinterpret_cast<const char*>(text), "abra", 4), 0);
}

TEST_F(SSLTest, DISABLED_OnTSL2Protocol_EcncryptionFail) {
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
            client_ctx->StartHandshake(&kClientBuf, &client_buf_len));

  while (!server_ctx->IsInitCompleted()) {
    ASSERT_FALSE(NULL == kClientBuf);
    ASSERT_LT(0u, client_buf_len);
    ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
              server_ctx->DoHandshakeStep(
                  kClientBuf, client_buf_len, &kServerBuf, &server_buf_len));
    ASSERT_FALSE(NULL == kServerBuf);
    ASSERT_LT(0u, server_buf_len);

    ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
              client_ctx->DoHandshakeStep(
                  kServerBuf, server_buf_len, &kClientBuf, &client_buf_len));
  }
  // Expect empty buffers after init complete
  ASSERT_TRUE(NULL == kClientBuf);
  ASSERT_EQ(0u, client_buf_len);
  // Expect both side initialization complete
  EXPECT_TRUE(client_ctx->IsInitCompleted());
  EXPECT_TRUE(server_ctx->IsInitCompleted());

  // Encrypt text on client side
  const uint8_t* text = reinterpret_cast<const uint8_t*>("abra");
  const uint8_t* encrypted_text = 0;
  size_t text_len = 4;
  size_t encrypted_text_len;
  EXPECT_TRUE(client_ctx->Encrypt(
      text, text_len, &encrypted_text, &encrypted_text_len));
  ASSERT_NE(reinterpret_cast<void*>(NULL), encrypted_text);
  ASSERT_LT(0u, encrypted_text_len);

  std::vector<uint8_t> broken(encrypted_text,
                              encrypted_text + encrypted_text_len);
  // Broke message
  broken[encrypted_text_len / 2] ^= 0xFF;

  const uint8_t* out_text;
  size_t out_text_size;
  // Decrypt broken text on server side
  EXPECT_FALSE(server_ctx->Decrypt(
      &broken[0], broken.size(), &out_text, &out_text_size));

  // Check after broken message that server encryption and decryption fail
  // Encrypte message on server side
  EXPECT_FALSE(server_ctx->Decrypt(
      encrypted_text, encrypted_text_len, &out_text, &out_text_size));
  EXPECT_FALSE(server_ctx->Encrypt(
      text, text_len, &encrypted_text, &encrypted_text_len));
}

TEST_P(SSLTestParam, ClientAndServerNotTLSv1_2_HandshakeFailed) {
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_AbnormalFail,
            client_ctx->StartHandshake(&kClientBuf, &client_buf_len));
  EXPECT_TRUE(NULL == kClientBuf);
  EXPECT_EQ(0u, client_buf_len);
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
            server_ctx->DoHandshakeStep(
                kClientBuf, client_buf_len, &kServerBuf, &server_buf_len));
  EXPECT_TRUE(NULL == kServerBuf);
  EXPECT_EQ(0u, server_buf_len);

  EXPECT_FALSE(server_ctx->IsInitCompleted());
}

INSTANTIATE_TEST_CASE_P(
    ServerProtocolTLSv12,
    SSLTestForTLS1_2,
    ::testing::Values(ProtocolAndCipher(security_manager::TLSv1,
                                        security_manager::TLSv1_2,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::TLSv1_1,
                                        security_manager::TLSv1_2,
                                        kFordCipher,
                                        kFordCipher),
                      ProtocolAndCipher(security_manager::SSLv3,
                                        security_manager::TLSv1_2,
                                        kFordCipher,
                                        kFordCipher)));

TEST_P(SSLTestForTLS1_2, HandshakeFailed) {
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
            client_ctx->StartHandshake(&kClientBuf, &client_buf_len));
  EXPECT_FALSE(NULL == kClientBuf);
  ASSERT_LT(0u, client_buf_len);
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_AbnormalFail,
            server_ctx->DoHandshakeStep(
                kClientBuf, client_buf_len, &kServerBuf, &server_buf_len));
  EXPECT_TRUE(NULL == kServerBuf);
  EXPECT_EQ(0u, server_buf_len);

  EXPECT_FALSE(server_ctx->IsInitCompleted());
}

}  // namespace ssl_context_test
}  // namespace components
}  // namespace test
