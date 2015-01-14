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

#include "security_manager/crypto_manager_impl.h"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "security_manager/security_manager.h"
#include "utils/logger.h"
#include "utils/atomic.h"

#define TLS1_1_MINIMAL_VERSION            0x1000103fL
#define CONST_SSL_METHOD_MINIMAL_VERSION  0x00909000L

namespace security_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "CryptoManagerImpl")

uint32_t CryptoManagerImpl::instance_count_ = 0;

CryptoManagerImpl::CryptoManagerImpl()
    : context_(NULL), mode_(CLIENT) {
}

bool CryptoManagerImpl::Init(Mode mode,
                             Protocol protocol,
                             const std::string &cert_filename,
                             const std::string &key_filename,
                             const std::string &ciphers_list,
                             bool verify_peer) {
  if (atomic_post_inc(&instance_count_) == 0) {
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
  }

  mode_ = mode;
  const bool is_server = (mode == SERVER);
#if OPENSSL_VERSION_NUMBER < CONST_SSL_METHOD_MINIMAL_VERSION
  SSL_METHOD *method;
#else
  const SSL_METHOD *method;
#endif
  switch (protocol) {
    case SSLv3:
      method = is_server ?
          SSLv3_server_method() :
          SSLv3_client_method();
      break;
    case TLSv1:
      method = is_server ?
          TLSv1_server_method() :
          TLSv1_client_method();
      break;
    case TLSv1_1:
#if OPENSSL_VERSION_NUMBER < TLS1_1_MINIMAL_VERSION
      LOG4CXX_WARN(logger_,
                   "OpenSSL has no TLSv1.1 with version lower 1.0.1, set TLSv1.0");
      method = is_server ?
          TLSv1_server_method() :
          TLSv1_client_method();
#else
      method = is_server ?
          TLSv1_1_server_method() :
          TLSv1_1_client_method();
#endif
      break;
    case TLSv1_2:
#if OPENSSL_VERSION_NUMBER < TLS1_1_MINIMAL_VERSION
      LOG4CXX_WARN(logger_,
                   "OpenSSL has no TLSv1.2 with version lower 1.0.1, set TLSv1.0");
      method = is_server ?
          TLSv1_server_method() :
          TLSv1_client_method();
#else
      method = is_server ?
          TLSv1_2_server_method() :
          TLSv1_2_client_method();
#endif
      break;
    default:
      LOG4CXX_ERROR(logger_, "Unknown protocol: " << protocol);
      return false;
  }
  context_ = SSL_CTX_new(method);

  // Disable SSL2 as deprecated
  SSL_CTX_set_options(context_, SSL_OP_NO_SSLv2);

  if (cert_filename.empty()) {
    LOG4CXX_WARN(logger_, "Empty certificate path");
  } else {
    LOG4CXX_INFO(logger_, "Certificate path: " << cert_filename);
    if (!SSL_CTX_use_certificate_file(context_, cert_filename.c_str(),
                                      SSL_FILETYPE_PEM)) {
      LOG4CXX_ERROR(logger_, "Could not use certificate " << cert_filename);
      return false;
    }
  }

  if (key_filename.empty()) {
    LOG4CXX_WARN(logger_, "Empty key path");
  } else {
    LOG4CXX_INFO(logger_, "Key path: " << key_filename);
    if (!SSL_CTX_use_PrivateKey_file(context_, key_filename.c_str(),
                                     SSL_FILETYPE_PEM)) {
      LOG4CXX_ERROR(logger_, "Could not use key " << key_filename);
      return false;
    }
    if (!SSL_CTX_check_private_key(context_)) {
      LOG4CXX_ERROR(logger_, "Could not use certificate " << cert_filename);
      return false;
    }
  }

  if (ciphers_list.empty()) {
    LOG4CXX_WARN(logger_, "Empty ciphers list");
  } else {
    LOG4CXX_INFO(logger_, "Cipher list: " << ciphers_list);
    if (!SSL_CTX_set_cipher_list(context_, ciphers_list.c_str())) {
      LOG4CXX_ERROR(logger_, "Could not set cipher list: " << ciphers_list);
      return false;
    }
  }

  // TODO(EZamakhov): add loading SSL_VERIFY_FAIL_IF_NO_PEER_CERT from INI
  const int verify_mode = verify_peer
      ? SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT
      : SSL_VERIFY_NONE;
  SSL_CTX_set_verify(context_, verify_mode, NULL);

  return true;
}

void CryptoManagerImpl::Finish() {
  SSL_CTX_free(context_);
  if (atomic_post_dec(&instance_count_) == 1) {
    EVP_cleanup();
    ERR_free_strings();
  }
}

SSLContext* CryptoManagerImpl::CreateSSLContext() {
  if (context_ == NULL) {
    return NULL;
  }

  SSL *conn = SSL_new(context_);
  if (conn == NULL)
    return NULL;

  if (mode_ == SERVER) {
    SSL_set_accept_state(conn);
  } else {
    SSL_set_connect_state(conn);
  }
  return new SSLContextImpl(conn, mode_);
}

void CryptoManagerImpl::ReleaseSSLContext(SSLContext *context) {
  delete context;
}

std::string CryptoManagerImpl::LastError() const {
  if (!context_) {
    return std::string("Initialization is not completed");
  }
  const char *reason = ERR_reason_error_string(ERR_get_error());
  return std::string(reason ? reason : "");
}

}  // namespace security_manager
