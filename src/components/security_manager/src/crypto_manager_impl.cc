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

#include "security_manager/crypto_manager_impl.h"

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace security_manager {

bool CryptoManagerImpl::Init() {
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();
  SSL_library_init();

  context_ = SSL_CTX_new(SSLv23_server_method());
  if (!SSL_CTX_use_certificate_file(context_, "mycert.pem", SSL_FILETYPE_PEM)) {
    return false;
  }
  if (!SSL_CTX_use_PrivateKey_file(context_, "mykey.pem", SSL_FILETYPE_PEM)) {
    return false;
  }
  if (!SSL_CTX_check_private_key(context_)) {
    return false;
  }
  SSL_CTX_set_cipher_list(context_, "ALL");
  SSL_CTX_set_verify(context_, SSL_VERIFY_NONE, NULL);
  //SSL_CTX_set_options(context_, SSL_OP_NO_SSLv2);

  return true;
}

SSLContext * CryptoManagerImpl::CreateSSLContext() {
  SSL* conn = SSL_new(context_);
  if (conn == NULL)
    return NULL;

  BIO *bioIn = BIO_new(BIO_s_mem());
  BIO *bioOut = BIO_new(BIO_s_mem());
  SSL_set_bio(conn, bioIn, bioOut);

  SSL_set_accept_state(conn);
  int ret = SSL_accept(conn);
  long error = ERR_get_error();
  const char *errstr = ERR_reason_error_string(error);
  return new SSLContextImpl(conn, bioIn, bioOut);
}

void CryptoManagerImpl::ReleaseSSLContext(SSLContext* context) {
  delete context;
}

} // namespace security_manager
