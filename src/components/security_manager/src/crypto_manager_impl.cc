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
#include <openssl/pkcs12.h>

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include "security_manager/security_manager.h"

#include "utils/logger.h"
#include "utils/atomic.h"
#include "utils/macro.h"
#include "utils/scope_guard.h"
#include "utils/date_time.h"

#define TLS1_1_MINIMAL_VERSION 0x1000103fL
#define CONST_SSL_METHOD_MINIMAL_VERSION 0x00909000L

namespace security_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "SecurityManager")

uint32_t CryptoManagerImpl::instance_count_ = 0;
sync_primitives::Lock CryptoManagerImpl::instance_lock_;

namespace {
int debug_callback(int preverify_ok, X509_STORE_CTX* ctx) {
  if (!preverify_ok) {
    const int error = X509_STORE_CTX_get_error(ctx);
    if (error == X509_V_ERR_CERT_NOT_YET_VALID ||
        error == X509_V_ERR_CERT_HAS_EXPIRED) {
      // return success result code instead of error because start
      // and expiration cert dates will be checked by SDL
      return 1;
    }
    LOG4CXX_WARN(logger_,
                 "Certificate verification failed with error "
                     << error << " \"" << X509_verify_cert_error_string(error)
                     << '"');
  }
  return preverify_ok;
}

void free_ctx(SSL_CTX** ctx) {
  if (ctx) {
    SSL_CTX_free(*ctx);
    *ctx = NULL;
  }
}
}

CryptoManagerImpl::CryptoManagerImpl(
    const utils::SharedPtr<const CryptoManagerSettings> set)
    : settings_(set), context_(NULL) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(instance_lock_);
  instance_count_++;
  if (instance_count_ == 1) {
    LOG4CXX_DEBUG(logger_, "Openssl engine initialization");
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
  }
  InitCertExpTime();
}

CryptoManagerImpl::~CryptoManagerImpl() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(instance_lock_);
  LOG4CXX_DEBUG(logger_, "Deinitilization");
  if (!context_) {
    LOG4CXX_WARN(logger_, "Manager is not initialized");
  } else {
    SSL_CTX_free(context_);
  }
  instance_count_--;
  if (instance_count_ == 0) {
    LOG4CXX_DEBUG(logger_, "Openssl engine deinitialization");
    EVP_cleanup();
    ERR_free_strings();
  }
}

bool CryptoManagerImpl::Init() {
  LOG4CXX_AUTO_TRACE(logger_);

  const Mode mode = get_settings().security_manager_mode();
  const bool is_server = (mode == SERVER);
  if (is_server) {
    LOG4CXX_DEBUG(logger_, "Server mode");
  } else {
    LOG4CXX_DEBUG(logger_, "Client mode");
  }
  LOG4CXX_DEBUG(logger_,
                "Peer verification "
                    << (get_settings().verify_peer() ? "enabled" : "disabled"));
  LOG4CXX_DEBUG(logger_,
                "CA certificate file is \"" << get_settings().ca_cert_path()
                                            << '"');

#if OPENSSL_VERSION_NUMBER < CONST_SSL_METHOD_MINIMAL_VERSION
  SSL_METHOD* method;
#else
  const SSL_METHOD* method;
#endif
  switch (get_settings().security_manager_protocol_name()) {
    case SSLv3:
      method = is_server ? SSLv3_server_method() : SSLv3_client_method();
      break;
    case TLSv1:
      method = is_server ? TLSv1_server_method() : TLSv1_client_method();
      break;
    case TLSv1_1:
#if OPENSSL_VERSION_NUMBER < TLS1_1_MINIMAL_VERSION
      LOG4CXX_WARN(
          logger_,
          "OpenSSL has no TLSv1.1 with version lower 1.0.1, set TLSv1.0");
      method = is_server ? TLSv1_server_method() : TLSv1_client_method();
#else
      method = is_server ? TLSv1_1_server_method() : TLSv1_1_client_method();
#endif
      break;
    case TLSv1_2:
#if OPENSSL_VERSION_NUMBER < TLS1_1_MINIMAL_VERSION
      LOG4CXX_WARN(
          logger_,
          "OpenSSL has no TLSv1.2 with version lower 1.0.1, set TLSv1.0");
      method = is_server ? TLSv1_server_method() : TLSv1_client_method();
#else
      method = is_server ? TLSv1_2_server_method() : TLSv1_2_client_method();
#endif
      break;
    default:
      LOG4CXX_ERROR(logger_,
                    "Unknown protocol: "
                        << get_settings().security_manager_protocol_name());
      return false;
  }
  if (context_) {
    free_ctx(&context_);
  }
  context_ = SSL_CTX_new(method);

  utils::ScopeGuard guard = utils::MakeGuard(free_ctx, &context_);

  // Disable SSL2 as deprecated
  SSL_CTX_set_options(context_, SSL_OP_NO_SSLv2);

  set_certificate(get_settings().certificate_data());

  if (get_settings().ciphers_list().empty()) {
    LOG4CXX_WARN(logger_, "Empty ciphers list");
  } else {
    LOG4CXX_DEBUG(logger_, "Cipher list: " << get_settings().ciphers_list());
    if (!SSL_CTX_set_cipher_list(context_,
                                 get_settings().ciphers_list().c_str())) {
      LOG4CXX_ERROR(
          logger_,
          "Could not set cipher list: " << get_settings().ciphers_list());
      return false;
    }
  }

  if (get_settings().ca_cert_path().empty()) {
    LOG4CXX_WARN(logger_, "Setting up empty CA certificate location");
  }

  LOG4CXX_DEBUG(logger_, "Setting up CA certificate location");
  const int result = SSL_CTX_load_verify_locations(
      context_, NULL, get_settings().ca_cert_path().c_str());

  if (!result) {
    const unsigned long error = ERR_get_error();
    UNUSED(error);
    LOG4CXX_WARN(logger_,
                 "Wrong certificate file '"
                     << get_settings().ca_cert_path() << "', err 0x" << std::hex
                     << error << " \"" << ERR_reason_error_string(error)
                     << '"');
  }

  guard.Dismiss();

  const int verify_mode =
      get_settings().verify_peer()
          ? SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT
          : SSL_VERIFY_NONE;
  LOG4CXX_DEBUG(logger_,
                "Setting up peer verification in mode: " << verify_mode);
  SSL_CTX_set_verify(context_, verify_mode, &debug_callback);
  return true;
}

bool CryptoManagerImpl::OnCertificateUpdated(const std::string& data) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!context_) {
    LOG4CXX_WARN(logger_, "Not initialized");
    return false;
  }

  return set_certificate(data);
}

SSLContext* CryptoManagerImpl::CreateSSLContext() {
  if (context_ == NULL) {
    return NULL;
  }

  SSL* conn = SSL_new(context_);
  if (conn == NULL)
    return NULL;

  if (get_settings().security_manager_mode() == SERVER) {
    SSL_set_accept_state(conn);
  } else {
    SSL_set_connect_state(conn);
  }
  return new SSLContextImpl(conn,
                            get_settings().security_manager_mode(),
                            get_settings().maximum_payload_size());
}

void CryptoManagerImpl::ReleaseSSLContext(SSLContext* context) {
  delete context;
}

std::string CryptoManagerImpl::LastError() const {
  if (!context_) {
    return std::string("Initialization is not completed");
  }
  const char* reason = ERR_reason_error_string(ERR_get_error());
  return std::string(reason ? reason : "");
}

bool CryptoManagerImpl::IsCertificateUpdateRequired(
    const time_t system_time, const time_t certificates_time) const {
  LOG4CXX_AUTO_TRACE(logger_);

  const double seconds = difftime(certificates_time, system_time);

  LOG4CXX_DEBUG(
      logger_, "Certificate UTC time: " << asctime(gmtime(&certificates_time)));

  LOG4CXX_DEBUG(logger_, "Host UTC time: " << asctime(gmtime(&system_time)));
  LOG4CXX_DEBUG(logger_, "Seconds before expiration: " << seconds);
  if (seconds < 0) {
    LOG4CXX_WARN(logger_, "Certificate is already expired.");
    return true;
  }

  return seconds <= (get_settings().update_before_hours() *
                     date_time::DateTime::SECONDS_IN_HOUR);
}

const CryptoManagerSettings& CryptoManagerImpl::get_settings() const {
  return *settings_;
}

bool CryptoManagerImpl::set_certificate(const std::string& cert_data) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (cert_data.empty()) {
    LOG4CXX_WARN(logger_, "Empty certificate");
    return false;
  }

  BIO* bio_cert =
      BIO_new_mem_buf(const_cast<char*>(cert_data.c_str()), cert_data.length());

  utils::ScopeGuard bio_guard = utils::MakeGuard(BIO_free, bio_cert);
  UNUSED(bio_guard)

  X509* cert = NULL;
  PEM_read_bio_X509(bio_cert, &cert, 0, 0);

  EVP_PKEY* pkey = NULL;
  if (1 == BIO_reset(bio_cert)) {
    PEM_read_bio_PrivateKey(bio_cert, &pkey, 0, 0);
  } else {
    LOG4CXX_WARN(logger_,
                 "Unabled to reset BIO in order to read private key, "
                     << LastError());
  }

  if (NULL == cert || NULL == pkey) {
    LOG4CXX_WARN(logger_, "Either certificate or key not valid.");
    return false;
  }

  if (!SSL_CTX_use_certificate(context_, cert)) {
    LOG4CXX_WARN(logger_, "Could not use certificate: " << LastError());
    return false;
  }

  if (!SSL_CTX_use_PrivateKey(context_, pkey)) {
    LOG4CXX_ERROR(logger_, "Could not use key: " << LastError());
    return false;
  }

  if (!SSL_CTX_check_private_key(context_)) {
    LOG4CXX_ERROR(logger_, "Could not use certificate: " << LastError());
    return false;
  }

  X509_STORE* store = SSL_CTX_get_cert_store(context_);
  if (store) {
    X509* extra_cert = NULL;
    while ((extra_cert = PEM_read_bio_X509(bio_cert, NULL, 0, 0))) {
      if (extra_cert != cert) {
        LOG4CXX_DEBUG(logger_,
                      "Added new certificate to store: " << extra_cert);
        X509_STORE_add_cert(store, extra_cert);
      }
    }
  }

  LOG4CXX_DEBUG(logger_, "Certificate and key successfully updated");
  return true;
}

int CryptoManagerImpl::SSLContextImpl::get_number_from_char_buf(
    char* buf, int* idx) const {
  if (!idx) {
    return 0;
  }
  const int val = ((buf[*idx] - '0') * 10) + buf[(*idx) + 1] - '0';
  *idx = *idx + 2;
  return val;
}

void CryptoManagerImpl::InitCertExpTime() {
  strptime("1 Jan 1970 00:00:00", "%d %b %Y %H:%M:%S", &expiration_time_);
}

}  // namespace security_manager
