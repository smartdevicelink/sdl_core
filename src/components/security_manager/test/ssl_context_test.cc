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

#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/ssl.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

#include "gtest/gtest.h"
#include "security_manager/crypto_manager.h"
#include "security_manager/crypto_manager_impl.h"
#include "security_manager/ssl_context.h"
#include "utils/custom_string.h"

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
namespace ssl_context_test {
namespace custom_str = utils::custom_string;

class SSLTest : public testing::Test {
 protected:
  static void SetUpTestCase() {
    std::ifstream file("server/spt_credential.p12.enc");
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    crypto_manager = new security_manager::CryptoManagerImpl();
    const bool crypto_manager_initialization = crypto_manager->Init(
        security_manager::SERVER, security_manager::TLSv1_2, ss.str(),
        FORD_CIPHER, false, "", updates_before_hour);
    EXPECT_TRUE(crypto_manager_initialization);

    client_manager = new security_manager::CryptoManagerImpl();
    const bool client_manager_initialization = client_manager->Init(
        security_manager::CLIENT, security_manager::TLSv1_2, "", FORD_CIPHER,
        false, "", updates_before_hour);
    EXPECT_TRUE(client_manager_initialization);
  }

  static void TearDownTestCase() {
    delete crypto_manager;
    delete client_manager;
  }

  virtual void SetUp() {
    server_ctx = crypto_manager->CreateSSLContext();
    client_ctx = client_manager->CreateSSLContext();

    security_manager::SSLContext::HandshakeContext ctx;
    ctx.make_context("SPT", "client");
    server_ctx->SetHandshakeContext(ctx);

    ctx.expected_cn = "server";
    client_ctx->SetHandshakeContext(ctx);
  }

  virtual void TearDown() {
    crypto_manager->ReleaseSSLContext(server_ctx);
    client_manager->ReleaseSSLContext(client_ctx);
  }

  static security_manager::CryptoManager* crypto_manager;
  static security_manager::CryptoManager* client_manager;
  security_manager::SSLContext* server_ctx;
  security_manager::SSLContext* client_ctx;
};

security_manager::CryptoManager* SSLTest::crypto_manager;
security_manager::CryptoManager* SSLTest::client_manager;

// TODO(EZAMAKHOV): Split to SSL/TLS1/TLS1_1/TLS1_2 tests

TEST_F(SSLTest, BrokenHandshake) {
  const uint8_t* server_buf;
  const uint8_t* client_buf;
  size_t server_buf_len;
  size_t client_buf_len;
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
            client_ctx->StartHandshake(&client_buf, &client_buf_len));
  ASSERT_FALSE(client_buf == NULL);
  ASSERT_GT(client_buf_len, 0u);
  // Broke 3 bytes for get abnormal fail of handshake
  const_cast<uint8_t*>(client_buf)[0] ^= 0xFF;
  const_cast<uint8_t*>(client_buf)[client_buf_len / 2] ^= 0xFF;
  const_cast<uint8_t*>(client_buf)[client_buf_len - 1] ^= 0xFF;
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_AbnormalFail,
            server_ctx->DoHandshakeStep(client_buf, client_buf_len, &server_buf,
                                        &server_buf_len));
}

TEST_F(SSLTest, Positive) {
  const uint8_t* server_buf;
  const uint8_t* client_buf;
  size_t server_buf_len;
  size_t client_buf_len;

  ASSERT_EQ(client_ctx->StartHandshake(&client_buf, &client_buf_len),
            security_manager::SSLContext::Handshake_Result_Success);
  ASSERT_FALSE(client_buf == NULL);
  ASSERT_GT(client_buf_len, 0u);

  for (;;) {
    ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
              server_ctx->DoHandshakeStep(client_buf, client_buf_len,
                                          &server_buf, &server_buf_len));
    ASSERT_FALSE(server_buf == NULL);
    ASSERT_GT(server_buf_len, 0u);

    ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
              client_ctx->DoHandshakeStep(server_buf, server_buf_len,
                                          &client_buf, &client_buf_len));
    if (server_ctx->IsInitCompleted()) {
      break;
    }

    ASSERT_FALSE(client_buf == NULL);
    ASSERT_GT(client_buf_len, 0u);
  }
  // Expect empty buffers after init complete
  ASSERT_TRUE(client_buf == NULL);
  ASSERT_EQ(client_buf_len, 0u);
  // expect both side initialization complete
  EXPECT_TRUE(client_ctx->IsInitCompleted());
  EXPECT_TRUE(server_ctx->IsInitCompleted());

  // Encrypt text on client side
  const uint8_t* text = reinterpret_cast<const uint8_t*>("abra");
  const uint8_t* encrypted_text = 0;
  size_t text_len = 4;
  size_t encrypted_text_len;
  EXPECT_TRUE(client_ctx->Encrypt(text, text_len, &encrypted_text,
                                  &encrypted_text_len));

  ASSERT_NE(encrypted_text, reinterpret_cast<void*>(NULL));
  ASSERT_GT(encrypted_text_len, 0u);

  // Decrypt text on server side
  EXPECT_TRUE(server_ctx->Decrypt(encrypted_text, encrypted_text_len, &text,
                                  &text_len));
  ASSERT_NE(text, reinterpret_cast<void*>(NULL));
  ASSERT_GT(text_len, 0u);

  ASSERT_EQ(strncmp(reinterpret_cast<const char*>(text), "abra", 4), 0);
}

TEST_F(SSLTest, EcncryptionFail) {
  const uint8_t* server_buf;
  const uint8_t* client_buf;
  size_t server_buf_len;
  size_t client_buf_len;
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
            client_ctx->StartHandshake(&client_buf, &client_buf_len));

  while (!server_ctx->IsInitCompleted()) {
    ASSERT_FALSE(client_buf == NULL);
    ASSERT_GT(client_buf_len, 0u);
    ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
              server_ctx->DoHandshakeStep(client_buf, client_buf_len,
                                          &server_buf, &server_buf_len));
    ASSERT_FALSE(server_buf == NULL);
    ASSERT_GT(server_buf_len, 0u);

    ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
              client_ctx->DoHandshakeStep(server_buf, server_buf_len,
                                          &client_buf, &client_buf_len));
  }
  // expect empty buffers after init complete
  ASSERT_TRUE(client_buf == NULL);
  ASSERT_EQ(client_buf_len, 0u);
  // expect both side initialization complete
  EXPECT_TRUE(client_ctx->IsInitCompleted());
  EXPECT_TRUE(server_ctx->IsInitCompleted());

  // Encrypt text on client side
  const uint8_t* text = reinterpret_cast<const uint8_t*>("abra");
  const uint8_t* encrypted_text = 0;
  size_t text_len = 4;
  size_t encrypted_text_len;
  EXPECT_TRUE(client_ctx->Encrypt(text, text_len, &encrypted_text,
                                  &encrypted_text_len));
  ASSERT_NE(encrypted_text, reinterpret_cast<void*>(NULL));
  ASSERT_GT(encrypted_text_len, 0u);

  std::vector<uint8_t> broken(encrypted_text,
                              encrypted_text + encrypted_text_len);
  // Broke message
  broken[encrypted_text_len / 2] ^= 0xFF;

  const uint8_t* out_text;
  size_t out_text_size;
  // Decrypt broken text on server side
  EXPECT_FALSE(server_ctx->Decrypt(&broken[0], broken.size(), &out_text,
                                   &out_text_size));

  // Check after broken message that server encryption and decryption fail
  // Encrypte message on server side
  EXPECT_FALSE(server_ctx->Decrypt(encrypted_text, encrypted_text_len,
                                   &out_text, &out_text_size));
  EXPECT_FALSE(server_ctx->Encrypt(text, text_len, &encrypted_text,
                                   &encrypted_text_len));
}

}  // namespace ssl_context_test
}  // namespace components
}  // namespace test
