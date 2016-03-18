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
#include "security_manager/security_manager.h"

#include "utils/logger.h"
#include "utils/atomic.h"
#include "utils/macro.h"
#include "utils/scope_guard.h"

#define TLS1_1_MINIMAL_VERSION            0x1000103fL
#define CONST_SSL_METHOD_MINIMAL_VERSION  0x00909000L

namespace security_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "SecurityManager")

uint32_t CryptoManagerImpl::instance_count_ = 0;
sync_primitives::Lock CryptoManagerImpl::instance_lock_;

namespace {
  int debug_callback(int preverify_ok, X509_STORE_CTX *ctx) {
    if (!preverify_ok) {
      const int error = X509_STORE_CTX_get_error(ctx);
      UNUSED(error);
      LOG4CXX_WARN(
          logger_,
          "Certificate verification failed with error " << error
          << " \"" << X509_verify_cert_error_string(error) << '"');
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
    : settings_(set)
    , context_(NULL) {
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
  LOG4CXX_DEBUG(logger_, "Peer verification "
                << (get_settings().verify_peer() ? "enabled" : "disabled"));
  LOG4CXX_DEBUG(logger_, "CA certificate file is \""
                << get_settings().ca_cert_path() << '"');

#if OPENSSL_VERSION_NUMBER < CONST_SSL_METHOD_MINIMAL_VERSION
  SSL_METHOD *method;
#else
  const SSL_METHOD *method;
#endif
  switch (get_settings().security_manager_protocol_name()) {
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
      LOG4CXX_ERROR(logger_, "Unknown protocol: "
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
      if (!SSL_CTX_set_cipher_list(context_, get_settings().ciphers_list().c_str())) {
        LOG4CXX_ERROR(logger_, "Could not set cipher list: "
                      << get_settings().ciphers_list());
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
    LOG4CXX_WARN(
        logger_,
        "Wrong certificate file '" << get_settings().ca_cert_path()
        << "', err 0x" << std::hex << error
        << " \"" << ERR_reason_error_string(error) << '"');
  }

  guard.Dismiss();

  const int verify_mode =
      get_settings().verify_peer() ? SSL_VERIFY_PEER |
                                     SSL_VERIFY_FAIL_IF_NO_PEER_CERT
                                   : SSL_VERIFY_NONE;
  LOG4CXX_DEBUG(logger_, "Setting up peer verification in mode: " << verify_mode);
  SSL_CTX_set_verify(context_, verify_mode, &debug_callback);
  return true;
}

bool CryptoManagerImpl::OnCertificateUpdated(const std::string &data) {
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

  SSL *conn = SSL_new(context_);
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

bool CryptoManagerImpl::IsCertificateUpdateRequired() const {
  LOG4CXX_AUTO_TRACE(logger_);

  const time_t now = time(NULL);
  const time_t cert_date = mktime(&expiration_time_);

  const double seconds = difftime(cert_date, now);
  LOG4CXX_DEBUG(
      logger_,
      "Certificate time: " << asctime(&expiration_time_)
                           << ". Host time: " << asctime(localtime(&now))
                           << ". Seconds before expiration: " << seconds);
  return seconds <= get_settings().update_before_hours();
}

const CryptoManagerSettings& CryptoManagerImpl::get_settings() const {
  return *settings_;
}


bool CryptoManagerImpl::set_certificate(const std::string &cert_data) {
  if (cert_data.empty()) {
    LOG4CXX_WARN(logger_, "Empty certificate");
    return false;
  }

  BIO* bio = BIO_new(BIO_f_base64());
  BIO* bmem = BIO_new_mem_buf((char*)cert_data.c_str(), cert_data.length());
  bmem = BIO_push(bio, bmem);

  char* buf = new char[cert_data.length()];
  int len = BIO_read(bmem, buf, cert_data.length());


  BIO* bio_cert = BIO_new(BIO_s_mem());
  if (NULL == bio_cert) {
    LOG4CXX_WARN(logger_, "Unable to update certificate. BIO not created");
    return false;
  }

  utils::ScopeGuard bio_guard = utils::MakeGuard(BIO_free, bio_cert);
  UNUSED(bio_guard)
  int k = 0;
  if ((k = BIO_write(bio_cert, buf, len)) <= 0) {
    LOG4CXX_WARN(logger_, "Unable to write into BIO");
    return false;
  }

  PKCS12* p12 = d2i_PKCS12_bio(bio_cert, NULL);
  if(NULL == p12) {
    LOG4CXX_ERROR(logger_, "Unable to parse certificate");
    return false;
  }

  EVP_PKEY* pkey = NULL;
  X509* cert = NULL;
  PKCS12_parse(p12, NULL, &pkey, &cert, NULL);

  if (NULL == cert || NULL == pkey){
    LOG4CXX_WARN(logger_, "Either certificate or key not valid.");
    return false;
  }

  if (!SSL_CTX_use_certificate(context_, cert)) {
    LOG4CXX_WARN(logger_, "Could not use certificate");
    return false;
  }

  asn1_time_to_tm(X509_get_notAfter(cert));

  if (!SSL_CTX_use_PrivateKey(context_, pkey)) {
    LOG4CXX_ERROR(logger_, "Could not use key");
    return false;
  }
  if (!SSL_CTX_check_private_key(context_)) {
    LOG4CXX_ERROR(logger_, "Could not use certificate ");
    return false;
  }
  return true;
}

int CryptoManagerImpl::pull_number_from_buf(char *buf, int *idx) {
  if (!idx) { return 0; }
  const int val = ((buf[*idx] - '0') * 10) + buf[(*idx) + 1] - '0';
  *idx = *idx + 2;
  return val;
}

void CryptoManagerImpl::asn1_time_to_tm(ASN1_TIME *time) {
  char* buf = (char *)time->data;
  int index = 0;
  const int year = pull_number_from_buf(buf, &index);
  if (V_ASN1_GENERALIZEDTIME == time->type) {
    expiration_time_.tm_year = (year * 100 -1900) + pull_number_from_buf(buf, &index);
  } else {
    expiration_time_.tm_year = year < 50 ? year + 100 : year;
  }

  const int mon = pull_number_from_buf(buf, &index);
  const int day = pull_number_from_buf(buf, &index);
  const int hour = pull_number_from_buf(buf, &index);
  const int mn = pull_number_from_buf(buf, &index);

  expiration_time_.tm_mon = mon -1;
  expiration_time_.tm_mday = day;
  expiration_time_.tm_hour = hour;
  expiration_time_.tm_min = mn;

  if (buf[index] == 'Z') {
    expiration_time_.tm_sec = 0;
  }
  if ((buf[index] == '+') || (buf[index] == '-')) {
    const int mn = pull_number_from_buf(buf, &index);
    const int mn1 = pull_number_from_buf(buf, &index);
    expiration_time_.tm_sec = (mn * 3600) + (mn1 * 60);
  } else {
    const int sec =  pull_number_from_buf(buf, &index);
    expiration_time_.tm_sec = sec;
  }
}

}  // namespace security_manager
