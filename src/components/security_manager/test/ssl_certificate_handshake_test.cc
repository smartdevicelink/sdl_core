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
#include "utils/custom_string.h"

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
const std::string client_certificate = "client/client_credential.p12.enc";
const std::string server_certificate = "server/spt_credential.p12.enc";
const std::string server_unsigned_cert_file =
    "server/spt_credential_unsigned.p12.enc";
const std::string server_expired_cert_file =
    "server/spt_credential_expired.p12.enc";

const bool verify_peer = true;
const bool skip_peer_verification = false;

const size_t updates_before_hour = 24;

}  // namespace

class SSLHandshakeTest : public testing::Test {
 protected:
  void SetUp() OVERRIDE {
    server_manager = new security_manager::CryptoManagerImpl();
    ASSERT_TRUE(server_manager);
    client_manager = new security_manager::CryptoManagerImpl();
    ASSERT_TRUE(client_manager);
    server_ctx = NULL;
    client_ctx = NULL;
  }

  void TearDown() OVERRIDE {
    server_manager->ReleaseSSLContext(server_ctx);
    delete server_manager;
    client_manager->ReleaseSSLContext(client_ctx);
    delete client_manager;
  }

  bool InitServerManagers(security_manager::Protocol protocol,
                          const std::string& cert_filename,
                          const std::string& ciphers_list,
                          const bool verify_peer,
                          const std::string& cacertificate_path) {
    std::ifstream cert(cert_filename);
    std::stringstream ss;
    ss << cert.rdbuf();
    cert.close();
    const bool initialized = server_manager->Init(security_manager::SERVER,
                                                  protocol,
                                                  ss.str(),
                                                  ciphers_list,
                                                  verify_peer,
                                                  cacertificate_path,
                                                  updates_before_hour);
    if (!initialized) {
      return false;
    }

    server_ctx = server_manager->CreateSSLContext();

    if (!server_ctx) {
      return false;
    }

    security_manager::SSLContext::HandshakeContext ctx;
    server_ctx->SetHandshakeContext(ctx.make_context(custom_str::CustomString("SPT"),
                                                     custom_str::CustomString("client")));

    return true;
  }

  bool InitClientManagers(security_manager::Protocol protocol,
                          const std::string& cert_filename,
                          const std::string& ciphers_list,
                          const bool verify_peer,
                          const std::string& cacertificate_path) {
    std::ifstream cert(cert_filename);
    std::stringstream ss;
    ss << cert.rdbuf();
    cert.close();
    const bool initialized = client_manager->Init(security_manager::CLIENT,
                                                  protocol,
                                                  ss.str(),
                                                  ciphers_list,
                                                  verify_peer,
                                                  cacertificate_path,
                                                  updates_before_hour);
    if (!initialized) {
      return false;
    }

    client_ctx = client_manager->CreateSSLContext();
    if (!client_ctx) {
      return false;
    }

    security_manager::SSLContext::HandshakeContext ctx;
    client_ctx->SetHandshakeContext(ctx.make_context(
        custom_str::CustomString("SPT"), custom_str::CustomString("server")));

    return true;
  }

  void ResetConnections() {
    ASSERT_NO_THROW(server_ctx->ResetConnection());
    ASSERT_NO_THROW(client_ctx->ResetConnection());
  }

  void StartHandshake() {
    using security_manager::SSLContext;

    ASSERT_EQ(SSLContext::Handshake_Result_Success,
              client_ctx->StartHandshake(&client_buf, &client_buf_len));
    ASSERT_FALSE(client_buf == NULL);
    ASSERT_GT(client_buf_len, 0u);
  }

  void HandshakeProcedure_Success() {
    using security_manager::SSLContext;
    StartHandshake();

    while (true) {
      ASSERT_EQ(SSLContext::Handshake_Result_Success,
                server_ctx->DoHandshakeStep(
                    client_buf, client_buf_len, &server_buf, &server_buf_len))
          << ERR_reason_error_string(ERR_get_error());
      ASSERT_FALSE(server_buf == NULL);
      ASSERT_GT(server_buf_len, 0u);

      ASSERT_EQ(SSLContext::Handshake_Result_Success,
                client_ctx->DoHandshakeStep(
                    server_buf, server_buf_len, &client_buf, &client_buf_len))
          << ERR_reason_error_string(ERR_get_error());
      if (server_ctx->IsInitCompleted()) {
        break;
      }

      ASSERT_FALSE(client_buf == NULL);
      ASSERT_GT(client_buf_len, 0u);
    }
  }

  void HandshakeProcedure_ServerSideFail() {
    using security_manager::SSLContext;

    StartHandshake();

    while (true) {
      const SSLContext::HandshakeResult result = server_ctx->DoHandshakeStep(
          client_buf, client_buf_len, &server_buf, &server_buf_len);
      ASSERT_FALSE(server_ctx->IsInitCompleted())
          << "Expected server side handshake fail";

      // First few handsahke will be successful
      if (result != SSLContext::Handshake_Result_Success) {
        // Test successfully passed with handshake fail
        return;
      }
      ASSERT_FALSE(server_buf == NULL);
      ASSERT_GT(server_buf_len, 0u);

      ASSERT_EQ(SSLContext::Handshake_Result_Success,
                client_ctx->DoHandshakeStep(
                    server_buf, server_buf_len, &client_buf, &client_buf_len))
          << ERR_reason_error_string(ERR_get_error());
      ASSERT_FALSE(client_ctx->IsInitCompleted())
          << "Expected server side handshake fail";

      ASSERT_FALSE(client_buf == NULL);
      ASSERT_GT(client_buf_len, 0u);
    }
    FAIL() << "Expected server side handshake fail";
  }

  void HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::HandshakeResult expected_result) {
    using security_manager::SSLContext;

    StartHandshake();

    while (true) {
      ASSERT_EQ(SSLContext::Handshake_Result_Success,
                server_ctx->DoHandshakeStep(
                    client_buf, client_buf_len, &server_buf, &server_buf_len))
          << ERR_reason_error_string(ERR_get_error());

      ASSERT_FALSE(server_buf == NULL);
      ASSERT_GT(server_buf_len, 0u);

      const SSLContext::HandshakeResult result = client_ctx->DoHandshakeStep(
          server_buf, server_buf_len, &client_buf, &client_buf_len);
      ASSERT_FALSE(client_ctx->IsInitCompleted())
          << "Expected client side handshake fail";

      // First few handsahke will be successful
      if (result != SSLContext::Handshake_Result_Success) {
        // Test successfully passed with handshake fail
        ASSERT_EQ(expected_result, result);
        return;
      }

      ASSERT_FALSE(client_buf == NULL);
      ASSERT_GT(client_buf_len, 0u);
    }
    FAIL() << "Expected client side handshake fail";
  }

  security_manager::CryptoManager* server_manager;
  security_manager::CryptoManager* client_manager;
  security_manager::SSLContext* server_ctx;
  security_manager::SSLContext* client_ctx;

  const uint8_t* server_buf;
  const uint8_t* client_buf;
  size_t server_buf_len;
  size_t client_buf_len;
};

TEST_F(SSLHandshakeTest, NoVerification) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << client_manager->LastError();

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_F(SSLHandshakeTest, CAVerification_ServerSide) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << client_manager->LastError();

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_F(SSLHandshakeTest, CAVerification_ServerSide_NoCACertificate) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 "unex"))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << client_manager->LastError();

  GTEST_TRACE(HandshakeProcedure_ServerSideFail());
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager->LastError();

  GTEST_TRACE(ResetConnections());

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_F(SSLHandshakeTest, CAVerification_ClientSide) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager->LastError();

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_F(SSLHandshakeTest, CAVerification_ClientSide_NoCACertificate) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 "client_ca_cert_filename"))
      << client_manager->LastError();

  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_Fail));

  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager->LastError();

  GTEST_TRACE(ResetConnections());

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_F(SSLHandshakeTest, CAVerification_BothSides) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager->LastError();

  GTEST_TRACE(HandshakeProcedure_Success());
}

TEST_F(SSLHandshakeTest, UnsignedCert) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_unsigned_cert_file,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << client_manager->LastError();
  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_CertNotSigned));
}

TEST_F(SSLHandshakeTest, ExpiredCert) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_expired_cert_file,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager->LastError();

  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_CertExpired));
}

TEST_F(SSLHandshakeTest, AppNameAndAppIDInvalid) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 verify_peer,
                                 server_ca_cert_filename))
      << client_manager->LastError();

  security_manager::SSLContext::HandshakeContext ctx;
  client_ctx->SetHandshakeContext(ctx.make_context(
      custom_str::CustomString("server"), custom_str::CustomString("Wrong")));

  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_AppNameMismatch));

  ResetConnections();
  client_ctx->SetHandshakeContext(ctx.make_context(
      custom_str::CustomString("Wrong"), custom_str::CustomString("server")));

  GTEST_TRACE(HandshakeProcedure_ClientSideFail(
      security_manager::SSLContext::Handshake_Result_AppIDMismatch));
}

TEST_F(SSLHandshakeTest, NoVerification_ResetConnection) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 ""))
      << client_manager->LastError();

  const int times = 100;
  for (int i = 0; i < times; ++i) {
    // Expect success  handshake
    GTEST_TRACE(HandshakeProcedure_Success());

    // Reset SSl connections
    GTEST_TRACE(ResetConnections());
  }
}

TEST_F(SSLHandshakeTest, CAVerification_BothSides_ResetConnection) {
  ASSERT_TRUE(InitServerManagers(security_manager::TLSv1_2,
                                 server_certificate,
                                 "ALL",
                                 verify_peer,
                                 client_ca_cert_filename))
      << server_manager->LastError();
  ASSERT_TRUE(InitClientManagers(security_manager::TLSv1_2,
                                 client_certificate,
                                 "ALL",
                                 skip_peer_verification,
                                 server_ca_cert_filename))
      << client_manager->LastError();

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
