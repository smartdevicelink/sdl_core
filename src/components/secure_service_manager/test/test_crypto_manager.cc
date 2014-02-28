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
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <iostream>

#include "secure_service_manager/crypto_manager.h"
#include "secure_service_manager/crypto_manager_impl.h"
#include "secure_service_manager/ssl_context.h"

using namespace secure_service_manager;

bool isErrorFatal(SSL *connection, int res) {
  int error = SSL_get_error(connection, res);
  return (error != SSL_ERROR_WANT_READ &&
      error != SSL_ERROR_WANT_WRITE);
}

int main() {
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();
  SSL_library_init();

  SSL_CTX * ctx = SSL_CTX_new(SSLv23_client_method());
  SSL_CTX_set_cipher_list(ctx, "ALL");
  SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

  SSL * connection = SSL_new(ctx);

  BIO* bioIn = BIO_new(BIO_s_mem());
  BIO* bioOut = BIO_new(BIO_s_mem());
  SSL_set_bio(connection, bioIn, bioOut);

  SSL_set_connect_state(connection);


  CryptoManager* crypto_manager = new CryptoManagerImpl();
  crypto_manager->Init();

  SSLContext *server_ctx = crypto_manager->CreateSSLContext();

  int res = 0;

  char *outBuf = new char[1024 * 1024];
  char *inBuf = new char[1024 * 1024];

  for(;;) {
    res = SSL_do_handshake(connection);
    if (res < 0) {
      if (isErrorFatal(connection, res)) {
        break;
      }

      size_t outLen  = BIO_ctrl_pending(bioOut);
      if (outLen) {
        BIO_read(bioOut, outBuf, outLen);
      }
      size_t inLen = 1024 * 1024;
      size_t resp_size = server_ctx->DoHandshake(outBuf, outLen, inBuf, inLen);

      if (resp_size) {
        BIO_write(bioIn, inBuf, resp_size);
      }
    } else {
      break;
    }
  }

  if (res == 0) {
    std::cout << "Handshake failed" << std::endl;
  } else {
    std::cout << "Handshake finished" << std::endl;
  }

  BIO *bioF = BIO_new(BIO_f_ssl());
  BIO_set_ssl(bioF, connection, BIO_NOCLOSE);

  char text[] = "Hello, it's the text to be encrypted";
  BIO_write(bioF, text, sizeof(text));

  size_t len = BIO_ctrl_pending(bioOut);
  char *encryptedText = new char[1024];
  BIO_read(bioOut, encryptedText, len);

  std::cout << "Encrypted text: " << std::string(encryptedText, len) << std::endl;

  char *decryptedText= new char[1024];
  len = server_ctx->Decrypt(encryptedText, len, decryptedText, 1024);

  std::cout << "Decrypted text: " << std::string(decryptedText, len) << std::endl;

  len = server_ctx->Encrypt(text, sizeof(text), encryptedText, 1024);
  std::cout << "Encrypted text: " << std::string(encryptedText, len) << std::endl;


  BIO_write(bioIn, encryptedText, len);
  len = BIO_read(bioF, decryptedText, 1024);
  std::cout << "Decrypted text: " << std::string(decryptedText, len) << std::endl;

  EVP_cleanup();
}
