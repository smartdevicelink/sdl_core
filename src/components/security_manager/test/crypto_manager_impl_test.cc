/*
 * Copyright (c) 2014, Ford Motor Company
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "security_manager/crypto_manager.h"
#include "security_manager/crypto_manager_impl.h"
#include "security_manager/ssl_context.h"

#ifdef __QNXNTO__
#include <openssl/ssl3.h>
#else
#include <openssl/tls1.h>
#endif

#ifdef __QNXNTO__
#define FORD_CIPHER   SSL3_TXT_RSA_DES_192_CBC3_SHA
#else
// Used cipher from ford protocol requirement
#define FORD_CIPHER   TLS1_TXT_RSA_WITH_AES_256_GCM_SHA384
#endif

#define ALL_CIPHERS   "ALL"

namespace test {
namespace components {
namespace security_manager_test {

namespace {
bool isErrorFatal(SSL *connection, int res) {
  const int error = SSL_get_error(connection, res);
  return (error != SSL_ERROR_WANT_READ && error != SSL_ERROR_WANT_WRITE);
}
}
// TODO(EZamakhov): May be split to SSLContext and Cryptomanager tests (separate files)
// TODO(EZamakhov): add test for EnsureBufferSizeEnough
class SSLTest : public testing::Test {
 protected:
  static void SetUpTestCase() {
    crypto_manager = new security_manager::CryptoManagerImpl();
    const bool crypto_manager_initialization = crypto_manager->Init(
        security_manager::SERVER, security_manager::TLSv1_2, "mycert.pem",
        "mykey.pem", FORD_CIPHER, false);
    EXPECT_TRUE(crypto_manager_initialization);

    client_manager = new security_manager::CryptoManagerImpl();
    const bool client_manager_initialization = client_manager->Init(
        security_manager::CLIENT, security_manager::TLSv1_2, "", "",
        FORD_CIPHER,
        false);
    EXPECT_TRUE(client_manager_initialization);
  }

  static void TearDownTestCase() {
    crypto_manager->Finish();
    client_manager->Finish();
    delete crypto_manager;
    delete client_manager;
  }

  virtual void SetUp() {
    server_ctx = crypto_manager->CreateSSLContext();
    client_ctx = client_manager->CreateSSLContext();
  }

  virtual void TearDown() {
    crypto_manager->ReleaseSSLContext(server_ctx);
    client_manager->ReleaseSSLContext(client_ctx);
  }

  static security_manager::CryptoManager* crypto_manager;
  static security_manager::CryptoManager* client_manager;
  security_manager::SSLContext *server_ctx;
  security_manager::SSLContext *client_ctx;
};

security_manager::CryptoManager* SSLTest::crypto_manager;
security_manager::CryptoManager* SSLTest::client_manager;

TEST(CryptoManagerTest, UsingBeforeInit) {
  security_manager::CryptoManager *crypto_manager = new security_manager::CryptoManagerImpl();
  EXPECT_TRUE(crypto_manager->CreateSSLContext() == NULL);
  EXPECT_EQ(crypto_manager->LastError(), std::string ("Initialization is not completed"));
  delete crypto_manager;
}

TEST(CryptoManagerTest, WrongInit) {
  security_manager::CryptoManager *crypto_manager = new security_manager::CryptoManagerImpl();

  //We have to cast (-1) to security_manager::Protocol Enum to be accepted by crypto_manager->Init(...)
  security_manager::Protocol UNKNOWN = static_cast<security_manager::Protocol>(-1);

  // Unknown protocol version
  EXPECT_FALSE(crypto_manager->Init(security_manager::SERVER, UNKNOWN,
          "mycert.pem", "mykey.pem", FORD_CIPHER, false));

  EXPECT_FALSE(crypto_manager->LastError().empty());
  // Unexistent cert file
  EXPECT_FALSE(crypto_manager->Init(security_manager::SERVER, security_manager::TLSv1_2,
          "unexists_file.pem", "mykey.pem", FORD_CIPHER, false));
  EXPECT_FALSE(crypto_manager->LastError().empty());
  // Unexistent key file
  EXPECT_FALSE(crypto_manager->Init(security_manager::SERVER, security_manager::TLSv1_2,
          "mycert.pem", "unexists_file.pem", FORD_CIPHER, false));
  EXPECT_FALSE(crypto_manager->LastError().empty());
  // Unexistent cipher value
  EXPECT_FALSE(crypto_manager->Init(security_manager::SERVER, security_manager::TLSv1_2,
          "mycert.pem", "mykey.pem", "INVALID_UNKNOWN_CIPHER", false));
  EXPECT_FALSE(crypto_manager->LastError().empty());

   delete crypto_manager;
}

//TEST(CryptoManagerTest, CorrectInit) {
//  security_manager::CryptoManager *crypto_manager = new security_manager::CryptoManagerImpl();
//  // Empty cert and key values for SERVER
//  EXPECT_TRUE(crypto_manager->Init(security_manager::SERVER, security_manager::TLSv1_2,
//          "", "", FORD_CIPHER, false));
//  EXPECT_TRUE(crypto_manager->LastError().empty());
//  // Recall init
//  EXPECT_TRUE(crypto_manager->Init(security_manager::CLIENT, security_manager::TLSv1_2,
//          "", "", FORD_CIPHER, false));
//  EXPECT_TRUE(crypto_manager->LastError().empty());
//  // Recall init with other protocols
//  EXPECT_TRUE(crypto_manager->Init(security_manager::CLIENT, security_manager::TLSv1_1,
//          "", "", FORD_CIPHER, false));
//  EXPECT_TRUE(crypto_manager->LastError().empty());
//  EXPECT_TRUE(crypto_manager->Init(security_manager::CLIENT, security_manager::TLSv1,
//          "", "", FORD_CIPHER, false));
//  EXPECT_TRUE(crypto_manager->LastError().empty());

//  // Cipher value
//  EXPECT_TRUE(crypto_manager->Init(security_manager::SERVER, security_manager::TLSv1_2,
//          "mycert.pem", "mykey.pem", ALL_CIPHERS, false));
//  EXPECT_TRUE(crypto_manager->LastError().empty());
//  delete crypto_manager;
//}

TEST(CryptoManagerTest, ReleaseNull) {
  using security_manager::CryptoManager;
  using security_manager::CryptoManagerImpl;

  CryptoManager *cm = new CryptoManagerImpl();
  EXPECT_NO_THROW(cm->ReleaseSSLContext(NULL));
  delete cm;
}

TEST_F(SSLTest, BrokenHandshake) {
  const uint8_t *server_buf;
  const uint8_t *client_buf;
  size_t server_buf_len;
  size_t client_buf_len;
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_Success,
      client_ctx->StartHandshake(&client_buf,
          &client_buf_len));
  ASSERT_FALSE(client_buf == NULL);
  ASSERT_GT(client_buf_len, 0u);
  // Broke 3 bytes for get abnormal fail of handshake
  const_cast<uint8_t*>(client_buf)[0] ^= 0xFF;
  const_cast<uint8_t*>(client_buf)[client_buf_len / 2] ^= 0xFF;
  const_cast<uint8_t*>(client_buf)[client_buf_len - 1] ^= 0xFF;
  ASSERT_EQ(security_manager::SSLContext::Handshake_Result_AbnormalFail,
      server_ctx->DoHandshakeStep(client_buf,
          client_buf_len,
          &server_buf,
          &server_buf_len));
}

// TODO(EZamakhov): split to SSL/TLS1/1.1/1.2 tests
// TODO{ALeshin}: APPLINK-10846
//TEST_F(SSLTest, Positive) {

//  const uint8_t *server_buf;
//  const uint8_t *client_buf;
//  size_t server_buf_len;
//  size_t client_buf_len;
//  ASSERT_EQ(client_ctx->StartHandshake(&client_buf,
//          &client_buf_len),
//      security_manager::SSLContext::Handshake_Result_Success);
//  ASSERT_FALSE(client_buf == NULL);
//  ASSERT_GT(client_buf_len, 0u);

//  for (;;) {
//    ASSERT_EQ(server_ctx->DoHandshakeStep(client_buf,
//            client_buf_len,
//            &server_buf,
//            &server_buf_len),
//        security_manager::SSLContext::Handshake_Result_Success);
//    ASSERT_FALSE(server_buf == NULL);
//    ASSERT_GT(server_buf_len, 0u);

//    ASSERT_EQ(client_ctx->DoHandshakeStep(server_buf,
//            server_buf_len,
//            &client_buf,
//            &client_buf_len),
//        security_manager::SSLContext::Handshake_Result_Success);
//    if (server_ctx->IsInitCompleted()) {
//      break;
//    }

//    ASSERT_FALSE(client_buf == NULL);
//    ASSERT_GT(client_buf_len, 0u);
//  }
//  // expect empty buffers after init complete
//  ASSERT_TRUE(client_buf == NULL);
//  ASSERT_EQ(client_buf_len, 0u);
//  // expect both side initialization complete
//  EXPECT_TRUE(client_ctx->IsInitCompleted());
//  EXPECT_TRUE(server_ctx->IsInitCompleted());

//  // Encrypt text on client side
//  const uint8_t *text = reinterpret_cast<const uint8_t*>("abra");
//  const uint8_t *encrypted_text = 0;
//  size_t text_len = 4;
//  size_t encrypted_text_len;
//  EXPECT_TRUE(client_ctx->Encrypt(text, text_len, &encrypted_text, &encrypted_text_len));

//  ASSERT_NE(encrypted_text, (void*)NULL);
//  ASSERT_GT(encrypted_text_len, 0u);

//  // Decrypt text on server side
//  EXPECT_TRUE(server_ctx->Decrypt(encrypted_text, encrypted_text_len, &text, &text_len));
//  ASSERT_NE(text, (void*)NULL);
//  ASSERT_GT(text_len, 0u);

//  ASSERT_EQ(strncmp(reinterpret_cast<const char*>(text),
//          "abra",
//          4), 0);
//}

//TODO{Aleshin}: APPLINK-10846
//TEST_F(SSLTest, EcncryptionFail) {

//  const uint8_t *server_buf;
//  const uint8_t *client_buf;
//  size_t server_buf_len;
//  size_t client_buf_len;
//  ASSERT_EQ(client_ctx->StartHandshake(&client_buf,
//          &client_buf_len),
//      security_manager::SSLContext::Handshake_Result_Success);

//  while (!server_ctx->IsInitCompleted()) {
//    ASSERT_FALSE(client_buf == NULL);
//    ASSERT_GT(client_buf_len, 0u);
//    ASSERT_EQ(server_ctx->DoHandshakeStep(client_buf, client_buf_len,
//            &server_buf, &server_buf_len),
//        security_manager::SSLContext::Handshake_Result_Success);
//    ASSERT_FALSE(server_buf == NULL);
//    ASSERT_GT(server_buf_len, 0u);

//    ASSERT_EQ(client_ctx->DoHandshakeStep(server_buf, server_buf_len,
//            &client_buf, &client_buf_len),
//        security_manager::SSLContext::Handshake_Result_Success);
//  }
//  // expect empty buffers after init complete
//  ASSERT_TRUE(client_buf == NULL);
//  ASSERT_EQ(client_buf_len, 0u);
//  // expect both side initialization complete
//  EXPECT_TRUE(client_ctx->IsInitCompleted());
//  EXPECT_TRUE(server_ctx->IsInitCompleted());

//  // Encrypt text on client side
//  const uint8_t *text = reinterpret_cast<const uint8_t*>("abra");
//  const uint8_t *encrypted_text = 0;
//  size_t text_len = 4;
//  size_t encrypted_text_len;
//  EXPECT_TRUE(client_ctx->Encrypt(text, text_len, &encrypted_text, &encrypted_text_len));
//  ASSERT_NE(encrypted_text, (void*)NULL);
//  ASSERT_GT(encrypted_text_len, 0u);

//  std::vector<uint8_t> broken(encrypted_text, encrypted_text + encrypted_text_len);
//  // Broke message
//  broken[encrypted_text_len / 2] ^= 0xFF;

//  const uint8_t *out_text;
//  size_t out_text_size;
//  // Decrypt broken text on server side
//  EXPECT_FALSE(server_ctx->Decrypt(&broken[0], broken.size(), &out_text, &out_text_size));

//  // Check after broken message that server encryption and decryption fail
//  // Encrypte message on server side
//  EXPECT_FALSE(server_ctx->Decrypt(encrypted_text, encrypted_text_len, &out_text, &out_text_size));
//  EXPECT_FALSE(server_ctx->Encrypt(text, text_len, &encrypted_text, &encrypted_text_len));
//}

/*
 TEST_F(SSLTest, DISABLED_BadData) {
 using security_manager::LastError;
 int res = 0;

 uint8_t *outBuf = new uint8_t[1024 * 1024];
 const uint8_t *inBuf;

 for(;;) {
 res = SSL_do_handshake(connection);
 if (res >= 0) {
 break;
 }

 if (isErrorFatal(connection, res)) {
 break;
 }

 size_t outLen  = BIO_ctrl_pending(bioOut);
 if (outLen) {
 BIO_read(bioOut, outBuf, outLen);
 }
 size_t inLen;
 server_ctx->DoHandshakeStep(outBuf, outLen, &inBuf, &inLen);
 EXPECT_TRUE(inBuf != NULL);

 if (inLen) {
 BIO_write(bioIn, inBuf, inLen);
 }
 }
 delete[] outBuf;

 EXPECT_EQ(res, 1);

 BIO *bioF = BIO_new(BIO_f_ssl());
 BIO_set_ssl(bioF, connection, BIO_NOCLOSE);

 const char *text = "Hello, it's the text to be encrypted";
 uint8_t *encryptedText = new uint8_t[1024];
 const   uint8_t *decryptedText;
 size_t text_len;

 // Encrypt text on client side
 BIO_write(bioF, text, sizeof(text));
 text_len = BIO_ctrl_pending(bioOut);
 size_t len = BIO_read(bioOut, encryptedText, text_len);

 // Make improvements
 encryptedText[len / 3] ^= 0x80;

 // Decrypt text on server
 server_ctx->Decrypt(encryptedText, len, &decryptedText, &text_len);

 delete[] encryptedText;

 EXPECT_FALSE(decryptedText == NULL);
 EXPECT_GT(LastError().length(), 0u);
 delete[] encryptedText;
 }



 TEST_F(SSLTest, Positive2) {
 using security_manager::LastError;
 int res = 0;

 uint8_t *outBuf = new uint8_t[1024 * 1024];
 const uint8_t *inBuf;

 for(;;) {
 res = SSL_do_handshake(connection);
 if (res >= 0) {
 break;
 }

 if (isErrorFatal(connection, res)) {
 break;
 }

 size_t outLen  = BIO_ctrl_pending(bioOut);
 if (outLen) {
 BIO_read(bioOut, outBuf, outLen);
 }
 size_t inLen;
 server_ctx->DoHandshakeStep(outBuf, outLen, &inBuf, &inLen);
 EXPECT_TRUE(inBuf != NULL);

 if (inLen) {
 BIO_write(bioIn, inBuf, inLen);
 }
 }
 delete[] outBuf;

 EXPECT_EQ(res, 1);

 EXPECT_NE(SSL_is_init_finished(connection), 0u);

 BIO *bioF = BIO_new(BIO_f_ssl());
 BIO_set_ssl(bioF, connection, BIO_NOCLOSE);

 const int N =1000;
 int last_max = 0;
 int min_oh = N , max_oh = 0;
 for (int l = 1; l < N; ++l) {
 char *text = new char[l+1];
 text[l]='\0';
 uint8_t *encryptedText = new uint8_t[1024*N];
 const uint8_t *decryptedText;
 size_t text_len;
 // Encrypt text on client side
 BIO_write(bioF, text, l);
 text_len = BIO_ctrl_pending(bioOut);
 size_t len = BIO_read(bioOut, encryptedText, text_len);
 const int temp = len - l;
 min_oh = temp < min_oh ? temp : min_oh;
 max_oh = temp > max_oh ? temp : max_oh;
 if (last_max < len) {
 std::cout << l << "->" << len;
 if (l > 1) {
 std::cout << ", last overhead = " << last_max << "-" << l-1
 << " = " << last_max - (l - 1) << "bytes || ";
 std::cout << " overhead = " << len << "-" << l
 << " = " << len - l << "bytes";
 }
 std::cout << std::endl;
 last_max = len;

 // Decrypt text on server
 server_ctx->Decrypt(encryptedText, len, &decryptedText, &text_len);
 const_cast<uint8_t*>(decryptedText)[text_len] = 0;

 EXPECT_TRUE(decryptedText != NULL);
 EXPECT_EQ(strcmp(reinterpret_cast<const char*>(decryptedText), text), 0u);
 delete[] text;
 }
 std::cout << " min = " << min_oh << ", max = " << max_oh << std::endl;
 }
 //*/

}
  // namespace crypto_manager_test
} // namespace components
}  // namespace test

