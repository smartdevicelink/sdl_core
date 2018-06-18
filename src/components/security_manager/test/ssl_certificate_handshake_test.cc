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

#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "security_manager/crypto_manager_impl.h"
#include "security_manager/mock_security_manager_settings.h"
#include "utils/custom_string.h"

using ::testing::Return;
using ::testing::ReturnRef;

namespace test {
namespace components {
namespace ssl_handshake_test {
namespace custom_str = utils::custom_string;
// Use this macro for correct line printing
// in case of fail insize of the #method
#define GTEST_TRACE(method) \
  do {                      \
    SCOPED_TRACE("");       \
    method;                 \
  } while (false)

namespace {
const std::string server_ca_cert_filename = "server";
const std::string client_ca_cert_filename = "client";
const std::string client_certificate = "client/client_credential.pem";
const std::string server_certificate = "server/spt_credential.pem";
const std::string server_unsigned_cert_file =
    "server/spt_credential_unsigned.pem";
const std::string server_expired_cert_file =
    "server/spt_credential_expired.pem";

const bool verify_peer = true;
const bool skip_peer_verification = false;

}  // namespace
struct Protocol {
  security_manager::Protocol server_protocol;
  security_manager::Protocol client_protocol;

  Protocol(security_manager::Protocol s_protocol,
           security_manager::Protocol c_protocol)
      : server_protocol(s_protocol), client_protocol(c_protocol) {}
};

class SSLHandshakeTest : public testing::TestWithParam<Protocol> {
 protected:
  void SetUp() OVERRIDE {
    mock_server_manager_settings_ = new testing::NiceMock<
        security_manager_test::MockCryptoManagerSettings>();

    server_manager_ = new security_manager::CryptoManagerImpl(
        utils::SharedPtr<security_manager::CryptoManagerSettings>(
            mock_server_manager_settings_));
    ASSERT_TRUE(server_manager_);
    mock_client_manager_settings_ = new testing::NiceMock<
        security_manager_test::MockCryptoManagerSettings>();

    client_manager_ = new security_manager::CryptoManagerImpl(
        utils::SharedPtr<security_manager::CryptoManagerSettings>(
            mock_client_manager_settings_));
    ASSERT_TRUE(client_manager_);
    server_ctx_ = NULL;
    client_ctx_ = NULL;
  }

  void TearDown() OVERRIDE {
    server_manager_->ReleaseSSLContext(server_ctx_);
    delete server_manager_;
    client_manager_->ReleaseSSLContext(client_ctx_);
    delete client_manager_;
  }

  void SetServerInitialValues(const security_manager::Protocol protocol,
                              const std::string cert,
                              const std::string server_ciphers_list,
                              const bool verify_peer,
                              const std::string& ca_certificate_path) {
    server_certificate_ = cert;
    server_ciphers_list_ = server_ciphers_list;
    server_ca_certificate_path_ = ca_certificate_path;
    ON_CALL(*mock_server_manager_settings_, force_unprotected_service())
        .WillByDefault(ReturnRef(forced_unprotected_service_));
    ON_CALL(*mock_server_manager_settings_, force_protected_service())
        .WillByDefault(ReturnRef(forced_protected_service_));
    ON_CALL(*mock_server_manager_settings_, security_manager_mode())
        .WillByDefault(Return(security_manager::SERVER));
    ON_CALL(*mock_server_manager_settings_, security_manager_protocol_name())
        .WillByDefault(Return(protocol));
    ON_CALL(*mock_server_manager_settings_, certificate_data())
        .WillByDefault(ReturnRef(server_certificate_));
    ON_CALL(*mock_server_manager_settings_, ciphers_list())
        .WillByDefault(ReturnRef(server_ciphers_list_));
    ON_CALL(*mock_server_manager_settings_, ca_cert_path())
        .WillByDefault(ReturnRef(server_ca_certificate_path_));
    ON_CALL(*mock_server_manager_settings_, verify_peer())
        .WillByDefault(Return(verify_peer));
  }
  void SetClientInitialValues(const security_manager::Protocol protocol,
                              const std::string certificate,
                              const std::string client_ciphers_list,
                              const bool verify_peer,
                              const std::string& ca_certificate_path) {
    client_certificate_ = certificate;
    client_ciphers_list_ = client_ciphers_list;
    client_ca_certificate_path_ = ca_certificate_path;

    ON_CALL(*mock_client_manager_settings_, force_unprotected_service())
        .WillByDefault(ReturnRef(forced_unprotected_service_));
    ON_CALL(*mock_client_manager_settings_, force_protected_service())
        .WillByDefault(ReturnRef(forced_protected_service_));
    ON_CALL(*mock_client_manager_settings_, security_manager_mode())
        .WillByDefault(Return(security_manager::CLIENT));
    ON_CALL(*mock_client_manager_settings_, security_manager_protocol_name())
        .WillByDefault(Return(protocol));
    ON_CALL(*mock_client_manager_settings_, certificate_data())
        .WillByDefault(ReturnRef(client_certificate_));
    ON_CALL(*mock_client_manager_settings_, ciphers_list())
        .WillByDefault(ReturnRef(client_ciphers_list_));
    ON_CALL(*mock_client_manager_settings_, ca_cert_path())
        .WillByDefault(ReturnRef(client_ca_certificate_path_));
    ON_CALL(*mock_client_manager_settings_, verify_peer())
        .WillByDefault(Return(verify_peer));
  }

  bool InitServerManagers(security_manager::Protocol protocol,
                          const std::string& cert_filename,
                          const std::string& ciphers_list,
                          const bool verify_peer,
                          const std::string& ca_certificate_path) {
    std::ifstream cert(cert_filename);
    std::stringstream ss;
    if (cert.is_open()) {
      ss << cert.rdbuf();
    }
    cert.close();
    SetServerInitialValues(
        protocol, ss.str(), ciphers_list, verify_peer, ca_certificate_path);
    const bool initialized = server_manager_->Init();

    if (!initialized) {
      return false;
    }

    server_ctx_ = server_manager_->CreateSSLContext();

    if (!server_ctx_) {
      return false;
    }

    server_ctx_->SetHandshakeContext(
        security_manager::SSLContext::HandshakeContext(
            custom_str::CustomString("SPT"),
            custom_str::CustomString("client")));

    return true;
  }

  bool InitClientManagers(security_manager::Protocol protocol,
                          const std::string& cert_filename,
                          const std::string& ciphers_list,
                          const bool verify_peer,
                          const std::string& ca_certificate_path) {
    std::ifstream cert(cert_filename);
    std::stringstream certificate;
    if (cert.is_open()) {
      certificate << cert.rdbuf();
    }
    cert.close();
    SetClientInitialValues(protocol,
                           certificate.str(),
                           ciphers_list,
                           verify_peer,
                           ca_certificate_path);
    const bool initialized = client_manager_->Init();
    if (!initialized) {
      return false;
    }

    client_ctx_ = client_manager_->CreateSSLContext();
    if (!client_ctx_) {
      return false;
    }

    client_ctx_->SetHandshakeContext(
        security_manager::SSLContext::HandshakeContext(
            custom_str::CustomString("SPT"),
            custom_str::CustomString("server")));

    return true;
  }

  void ResetConnections() {
    ASSERT_NO_THROW(server_ctx_->ResetConnection());
    ASSERT_NO_THROW(client_ctx_->ResetConnection());
  }

  void StartHandshake() {
    using security_manager::SSLContext;

    ASSERT_EQ(SSLContext::Handshake_Result_Success,
              client_ctx_->StartHandshake(&client_buf_, &client_buf_len_));
    ASSERT_FALSE(client_buf_ == NULL);
    ASSERT_GT(client_buf_len_, 0u);
  }

  void HandshakeProcedure_Success() {
    using security_manager::SSLContext;
    StartHandshake();

    while (true) {
      ASSERT_EQ(
          SSLContext::Handshake_Result_Success,
          server_ctx_->DoHandshakeStep(
              client_buf_, client_buf_len_, &server_buf_, &server_buf_len_))
          << ERR_reason_error_string(ERR_get_error());
      ASSERT_FALSE(server_buf_ == NULL);
      ASSERT_GT(server_buf_len_, 0u);

      ASSERT_EQ(
          SSLContext::Handshake_Result_Success,
          client_ctx_->DoHandshakeStep(
              server_buf_, server_buf_len_, &client_buf_, &client_buf_len_))
          << ERR_reason_error_string(ERR_get_error());
      if (server_ctx_->IsInitCompleted()) {
        break;
      }

      ASSERT_FALSE(client_buf_ == NULL);
      ASSERT_GT(client_buf_len_, 0u);
    }
  }

  void HandshakeProcedure_ServerSideFail() {
    using security_manager::SSLContext;

    StartHandshake();

    while (true) {
      const SSLContext::HandshakeResult result = server_ctx_->DoHandshakeStep(
          client_buf_, client_buf_len_, &server_buf_, &server_buf_len_);
      ASSERT_FALSE(server_ctx_->IsInitCompleted())
          << "Expected server side handshake fail";

      // First few handshake will be successful
      if (result != SSLContext::Handshake_Result_Success) {
        // Test successfully passed with handshake fail
        return;
      }
      ASSERT_FALSE(server_buf_ == NULL);
      ASSERT_GT(server_buf_len_, 0u);

      ASSERT_EQ(
          SSLContext::Handshake_Result_Success,
          client_ctx_->DoHandshakeStep(
              server_buf_, server_buf_len_, &client_buf_, &client_buf_len_))
          << ERR_reason_error_string(ERR_get_error());
      ASSERT_FALSE(client_ctx_->IsInitCompleted())
          << "Expected server side handshake fail";

      ASSERT_FALSE(client_buf_ == NULL);
      ASSERT_GT(client_buf_len_, 0u);
    }
    FAIL() << "Expected server side handshake fail";
  }

  void HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::HandshakeResult expected_result) {
    using security_manager::SSLContext;

    StartHandshake();

    while (true) {
      ASSERT_EQ(
          SSLContext::Handshake_Result_Success,
          server_ctx_->DoHandshakeStep(
              client_buf_, client_buf_len_, &server_buf_, &server_buf_len_))
          << ERR_reason_error_string(ERR_get_error());

      ASSERT_FALSE(server_buf_ == NULL);
      ASSERT_GT(server_buf_len_, 0u);

      const SSLContext::HandshakeResult result = client_ctx_->DoHandshakeStep(
          server_buf_, server_buf_len_, &client_buf_, &client_buf_len_);
      ASSERT_FALSE(client_ctx_->IsInitCompleted())
          << "Expected client side handshake fail";

      // First few handsahke will be successful
      if (result != SSLContext::Handshake_Result_Success) {
        // Test successfully passed with handshake fail
        ASSERT_EQ(expected_result, result);
        return;
      }

      ASSERT_FALSE(client_buf_ == NULL);
      ASSERT_GT(client_buf_len_, 0u);
    }
    FAIL() << "Expected client side handshake fail";
  }

  security_manager::CryptoManager* server_manager_;
  security_manager::CryptoManager* client_manager_;
  security_manager::SSLContext* server_ctx_;
  security_manager::SSLContext* client_ctx_;
  testing::NiceMock<security_manager_test::MockCryptoManagerSettings>*
      mock_server_manager_settings_;
  testing::NiceMock<security_manager_test::MockCryptoManagerSettings>*
      mock_client_manager_settings_;

  const uint8_t* server_buf_;
  const uint8_t* client_buf_;
  size_t server_buf_len_;
  size_t client_buf_len_;

  std::string server_certificate_;
  std::string server_ciphers_list_;
  std::string server_ca_certificate_path_;

  std::string client_certificate_;
  std::string client_ciphers_list_;
  std::string client_ca_certificate_path_;

  const std::vector<int> forced_protected_service_;
  const std::vector<int> forced_unprotected_service_;
};

INSTANTIATE_TEST_CASE_P(
    CorrectProtocol,
    SSLHandshakeTest,
    ::testing::Values(
        Protocol(security_manager::TLSv1, security_manager::TLSv1),
        Protocol(security_manager::TLSv1_1, security_manager::TLSv1_1),
        Protocol(security_manager::TLSv1_2, security_manager::TLSv1_2),
        Protocol(security_manager::DTLSv1, security_manager::DTLSv1)));

TEST_P(SSLHandshakeTest, NoVerification) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << server_manager_->LastError();

  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << client_manager_->LastError();

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_P(SSLHandshakeTest, CAVerification_ServerSide) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << client_manager_->LastError();

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_P(SSLHandshakeTest, CAVerification_ServerSide_NoCACertificate) {
  ASSERT_TRUE(InitServerManagers(
      GetParam().server_protocol, "", "ALL", verify_peer, "unex"))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << client_manager_->LastError();

  GTEST_TRACE(HandshakeProcedure_ServerSideFail());
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager_->LastError();

  GTEST_TRACE(ResetConnections());

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_P(SSLHandshakeTest, CAVerification_ClientSide_NoCACertificate) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 "",
                                 "ALL",
                                 verify_peer,
                                 "client_ca_cert_filename"))
      << client_manager_->LastError();

  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_Fail));

  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager_->LastError();

  GTEST_TRACE(ResetConnections());

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_P(SSLHandshakeTest, CAVerification_BothSides) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager_->LastError();

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_P(SSLHandshakeTest, UnsignedCert) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_unsigned_cert_file,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << client_manager_->LastError();
  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_CertNotSigned));
}

TEST_P(SSLHandshakeTest, ExpiredCert) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_expired_cert_file,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager_->LastError();

  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_CertExpired));
}

TEST_P(SSLHandshakeTest, AppNameAndAppIDInvalid) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager_->LastError();

  client_ctx_->SetHandshakeContext(
      security_manager::SSLContext::HandshakeContext(
          custom_str::CustomString("server"),
          custom_str::CustomString("Wrong")));

  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_AppNameMismatch));

  ResetConnections();
  client_ctx_->SetHandshakeContext(
      security_manager::SSLContext::HandshakeContext(
          custom_str::CustomString("Wrong"),
          custom_str::CustomString("server")));

  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_AppIDMismatch));
}

TEST_P(SSLHandshakeTest, NoVerification_ResetConnection) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << client_manager_->LastError();

  const int times = 100;
  for (int i = 0; i < times; ++i) {
    // Expect success  handshake
    GTEST_TRACE(HandshakeProcedure_Success());

    // Reset SSl connections
    GTEST_TRACE(ResetConnections());
  }
}

TEST_P(SSLHandshakeTest, CAVerification_BothSides_ResetConnection) {
  ASSERT_TRUE(InitServerManagers(GetParam().server_protocol,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager_->LastError();
  ASSERT_TRUE(InitClientManagers(GetParam().client_protocol,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 server_ca_cert_filename))
      << client_manager_->LastError();

  const int times = 100;
  for (int i = 0; i < times; ++i) {
    // Expect success  handshake
    GTEST_TRACE(HandshakeProcedure_Success());

    // Reset SSl connections
    GTEST_TRACE(ResetConnections());
  }
}
// TODO(EZamakhov): add fail tests -broken or not full ca certificate chain

}  // namespace ssl_handshake_test
}  // namespace components
}  // namespace test
