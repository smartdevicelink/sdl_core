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
#include "security_manager/security_manager.h"

namespace security_manager {

log4cxx::LoggerPtr CryptoManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("CryptoManagerImpl"));

CryptoManagerImpl::CryptoManagerImpl()
    : context_(NULL),
      mode_(CLIENT) {
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();
  SSL_library_init();
}

bool CryptoManagerImpl::Init(Mode mode,
                             const std::string& cert_filename,
                             const std::string& key_filename,
                             const std::string& ciphers_list,
                             bool verify_peer) {

  mode_ = mode;
  if (mode == SERVER) {
    context_ = SSL_CTX_new(SSLv23_server_method());
  } else {
    context_ = SSL_CTX_new(SSLv23_client_method());
  }


  LOG4CXX_INFO(logger_, "Certificate path: " << cert_filename);
  if (!SSL_CTX_use_certificate_file(context_, cert_filename.c_str(), SSL_FILETYPE_PEM)) {
    LOG4CXX_ERROR(logger_, "Could not use certificate " << cert_filename);
    return false;
  }

  LOG4CXX_INFO(logger_, "Key path: " << key_filename);
  if (!SSL_CTX_use_PrivateKey_file(context_, key_filename.c_str(), SSL_FILETYPE_PEM)) {
    LOG4CXX_ERROR(logger_, "Could not use key " << key_filename);
    return false;
  }

  if (!SSL_CTX_check_private_key(context_)) {
    LOG4CXX_ERROR(logger_, "Could not use certificate " << cert_filename);
    return false;
  }

  LOG4CXX_INFO(logger_, "Cipher list: " << ciphers_list);
  if(!SSL_CTX_set_cipher_list(context_, ciphers_list.c_str())) {
    LOG4CXX_ERROR(logger_, "Could not set cipher list: " << ciphers_list);
    return false;
  }

  SSL_CTX_set_verify(context_, verify_peer ? SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT : SSL_VERIFY_NONE, NULL);

  // TODO (EZamakhov): is it legacy?
  //SSL_CTX_set_options(context_, SSL_OP_NO_SSLv2);
  return true;
}

void CryptoManagerImpl::Finish() {
  EVP_cleanup();
  ERR_free_strings();
}

SSLContext * CryptoManagerImpl::CreateSSLContext() {
  if (context_ == NULL) {
    return NULL;
  }

  SSL* conn = SSL_new(context_);
  if (conn == NULL)
    return NULL;

  if (mode_ == SERVER) {
    SSL_set_accept_state(conn);
  } else {
    SSL_set_connect_state(conn);
  }
  // TODO (EZamakhov) : add return NULL pointer on no keys
  return new SSLContextImpl(conn, mode_);
}

void CryptoManagerImpl::ReleaseSSLContext(SSLContext* context) {
  delete context;
}

} // namespace security_manager
