/*
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef CRYPTO_MANAGER_IMPL_TEST_H_
#define CRYPTO_MANAGER_IMPL_TEST_H_

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <gtest/gtest.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include "security_manager/crypto_manager.h"
#include "security_manager/crypto_manager_impl.h"
#include "security_manager/ssl_context.h"

namespace test {
namespace components {
namespace security_manager_test {

bool isErrorFatal(SSL *connection, int res) {
  int error = SSL_get_error(connection, res);
  return (error != SSL_ERROR_WANT_READ &&
      error != SSL_ERROR_WANT_WRITE);
}

class SSLTest : public testing::Test {
 protected:
  static void SetUpTestCase() {
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();

    ctx = SSL_CTX_new(SSLv23_client_method());
    SSL_CTX_set_cipher_list(ctx, "ALL");
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

    crypto_manager = new security_manager::CryptoManagerImpl();
    crypto_manager->Init(security_manager::SERVER, "mycert.pem", "mykey.pem", "ALL", false);
  }

  static void TearDownTestCase() {
    SSL_CTX_free(ctx);
    EVP_cleanup();

    crypto_manager->Finish();
    delete crypto_manager;
  }

  virtual void SetUp() {
    connection = SSL_new(ctx);
    bioIn = BIO_new(BIO_s_mem());
    bioOut = BIO_new(BIO_s_mem());
    SSL_set_bio(connection, bioIn, bioOut);
    SSL_set_connect_state(connection);

    server_ctx = crypto_manager->CreateSSLContext();
  }

  virtual void TearDown() {
    crypto_manager->ReleaseSSLContext(server_ctx);
    SSL_shutdown(connection);
    SSL_free(connection);
  }

  static SSL_CTX * ctx;
  static security_manager::CryptoManager* crypto_manager;
  SSL *connection;
  BIO *bioIn;
  BIO *bioOut;
  security_manager::SSLContext *server_ctx;
};

SSL_CTX *SSLTest::ctx;
security_manager::CryptoManager* SSLTest::crypto_manager;

TEST(CryptoManagerTest, UsingBeforeInit) {
  security_manager::CryptoManager *crypto_manager = new security_manager::CryptoManagerImpl();
  security_manager::SSLContext* ctx = crypto_manager->CreateSSLContext();

  EXPECT_TRUE(ctx == NULL);
}

TEST(CryptoManagerTest, ReleaseNull) {
  using security_manager::CryptoManager;
  using security_manager::CryptoManagerImpl;

  CryptoManager *cm = new CryptoManagerImpl();
  EXPECT_NO_THROW(cm->ReleaseSSLContext(NULL));
}

TEST_F(SSLTest, BrokenHandshake) {
  using security_manager::LastError;

  int res = 0;

  char *outBuf = new char[1024 * 1024];
  char *inBuf;

  // FIXME (EZamakhov): fix infinity loop on wrong handshake
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
      // Make some data improvements
      outBuf[outLen / 2] ^= 0x80;
    }

    size_t inLen;
    inBuf = static_cast<char*>(server_ctx->DoHandshakeStep(outBuf, outLen, &inLen));
    EXPECT_TRUE(inBuf != NULL);

    if (inLen) {
      BIO_write(bioIn, inBuf, inLen);
    }
  }

  EXPECT_GT(LastError().length(), 0);
  EXPECT_EQ(res, 0);
}

TEST_F(SSLTest, BadData) {
  using security_manager::LastError;
  int res = 0;

  char *outBuf = new char[1024 * 1024];
  char *inBuf;

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
    inBuf = static_cast<char*>(server_ctx->DoHandshakeStep(outBuf, outLen, &inLen));
    EXPECT_TRUE(inBuf != NULL);

    if (inLen) {
      BIO_write(bioIn, inBuf, inLen);
    }
  }

  EXPECT_EQ(res, 1);

  BIO *bioF = BIO_new(BIO_f_ssl());
  BIO_set_ssl(bioF, connection, BIO_NOCLOSE);

  char text[] = "Hello, it's the text to be encrypted";
  char *encryptedText = new char[1024];
  char *decryptedText;
  size_t text_len;

  // Encrypt text on client side
  BIO_write(bioF, text, sizeof(text));
  text_len = BIO_ctrl_pending(bioOut);
  size_t len = BIO_read(bioOut, encryptedText, text_len);

  // Make improvements
  encryptedText[len / 3] ^= 0x80;

  // Decrypt text on server
  decryptedText = static_cast<char*>(server_ctx->Decrypt(encryptedText, len, &text_len));

  delete[] encryptedText;

  EXPECT_TRUE(decryptedText == NULL);
  EXPECT_GT(LastError().length(), 0);
}

TEST_F(SSLTest, Positive) {
  using security_manager::LastError;
  int res = 0;

  char *outBuf = new char[1024 * 1024];
  char *inBuf;

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
    inBuf = static_cast<char*>(server_ctx->DoHandshakeStep(outBuf, outLen, &inLen));
    EXPECT_TRUE(inBuf != NULL);

    if (inLen) {
      BIO_write(bioIn, inBuf, inLen);
    }
  }

  EXPECT_EQ(res, 1);

  EXPECT_NE(SSL_is_init_finished(connection), 0);

  BIO *bioF = BIO_new(BIO_f_ssl());
  BIO_set_ssl(bioF, connection, BIO_NOCLOSE);

  const char *text = "abra";
  char *encryptedText = new char[10240];
  char *decryptedText;
  size_t text_len;

  // Encrypt text on client side
  BIO_write(bioF, text, strlen(text));
  text_len = BIO_ctrl_pending(bioOut);
  size_t len = BIO_read(bioOut, encryptedText, text_len);

  // Decrypt text on server
  decryptedText = static_cast<char*>(server_ctx->Decrypt(encryptedText, len, &text_len));
  decryptedText[text_len] = 0;

  EXPECT_TRUE(decryptedText != NULL);
  EXPECT_EQ(strcmp(decryptedText, text), 0);

  text = "abra cadabra";

  // Encrypt text on client side
  BIO_write(bioF, text, strlen(text));
  text_len = BIO_ctrl_pending(bioOut);
  len = BIO_read(bioOut, encryptedText, text_len);

  // Decrypt text on server
  decryptedText = static_cast<char*>(server_ctx->Decrypt(encryptedText, len, &text_len));
  decryptedText[text_len] = 0;

  EXPECT_TRUE(decryptedText != NULL);
  EXPECT_EQ(strcmp(decryptedText, text), 0);

  // Encrypt text on server
  encryptedText = static_cast<char*>(server_ctx->Encrypt(text, strlen(text), &text_len));

  // Decrypt it on client
  BIO_write(bioIn, encryptedText, text_len);
  decryptedText = new char[BIO_ctrl_pending(bioF)];
  text_len = BIO_read(bioF, decryptedText, BIO_ctrl_pending(bioF));
  decryptedText[text_len] = 0;
  EXPECT_EQ(strcmp(decryptedText, text), 0);
  EXPECT_EQ(LastError().length(), 0);

  // Encrypt text on server
  encryptedText = static_cast<char*>(server_ctx->Encrypt(text, strlen(text), &text_len));

  // Decrypt it on client
  BIO_write(bioIn, encryptedText, text_len);
  decryptedText = new char[BIO_ctrl_pending(bioF)];
  text_len = BIO_read(bioF, decryptedText, BIO_ctrl_pending(bioF));
  decryptedText[text_len] = 0;
  EXPECT_EQ(strcmp(decryptedText, text), 0);
  EXPECT_EQ(LastError().length(), 0);
}
TEST_F(SSLTest, Positive2) {
  using security_manager::LastError;
  int res = 0;

  char *outBuf = new char[1024 * 1024];
  char *inBuf;

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
    inBuf = static_cast<char*>(server_ctx->DoHandshakeStep(outBuf, outLen, &inLen));
    EXPECT_TRUE(inBuf != NULL);

    if (inLen) {
      BIO_write(bioIn, inBuf, inLen);
    }
  }

  EXPECT_EQ(res, 1);

  EXPECT_NE(SSL_is_init_finished(connection), 0);

  BIO *bioF = BIO_new(BIO_f_ssl());
  BIO_set_ssl(bioF, connection, BIO_NOCLOSE);

  const int N =1000;
  int last_max = 0;
  int min_oh = N , max_oh = 0;
  for (int l = 1; l < N; ++l) {
    char *text = new char[l+1];
    text[l]='\0';
    char *encryptedText = new char[1024*N];
    char *decryptedText;
    size_t text_len;
    // Encrypt text on client side
    BIO_write(bioF, text, l);
    text_len = BIO_ctrl_pending(bioOut);
    size_t len = BIO_read(bioOut, encryptedText, text_len);
    const int temp = len - l;
    min_oh = temp < min_oh ? temp : min_oh;
    max_oh = temp > max_oh ? temp : max_oh;
    if(last_max < len) {
      std::cout << l << "->" << len;
      if(l>1) {
        std::cout << ", last overhead = " << last_max << "-" << l-1
                  << " = " << last_max - (l-1) << "bytes || ";
        std::cout << " overhead = " << len << "-" << l
                  << " = " << len - l << "bytes";
      }
      std::cout << std::endl;
      last_max = len;
    };

    // Decrypt text on server
    decryptedText = static_cast<char*>(server_ctx->Decrypt(encryptedText, len, &text_len));
    decryptedText[text_len] = 0;

    EXPECT_TRUE(decryptedText != NULL);
    EXPECT_EQ(strcmp(decryptedText, text), 0);
    }
  std::cout << " min = " << min_oh << ", max = " << max_oh << std::endl;
}

}  // namespace crypto_manager_test
}  // namespace components
}  // namespace test

#endif /* CRYPTO_MANAGER_IMPL_TEST_H_ */
