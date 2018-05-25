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
#include <algorithm>
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

bool CryptoManagerImpl::AreForceProtectionSettingsCorrect() const {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::vector<int>& forced_unprotected_services =
      get_settings().force_unprotected_service();
  const std::vector<int>& forced_protected_services =
      get_settings().force_protected_service();

  for (auto& item : forced_protected_services) {
    if (0 == item) {
      continue;
    }

    if (std::find(forced_unprotected_services.begin(),
                  forced_unprotected_services.end(),
                  item) != forced_unprotected_services.end()) {
      return false;
    }
  }
  return true;
}

bool CryptoManagerImpl::Init() {
  LOG4CXX_AUTO_TRACE(logger_);

  const Mode mode = get_settings().security_manager_mode();
  if (!AreForceProtectionSettingsCorrect()) {
    LOG4CXX_DEBUG(logger_, "Force protection settings of ini file are wrong!");
    return false;
  }
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
  const SSL_METHOD* method = NULL;
#endif
  switch (get_settings().security_manager_protocol_name()) {
    case SSLv3:
#ifdef OPENSSL_NO_SSL3
      LOG4CXX_WARN(logger_, "OpenSSL does not support SSL3 protocol");
      return false;
#else
      LOG4CXX_DEBUG(logger_, "SSLv3 is used");
      method = is_server ? SSLv3_server_method() : SSLv3_client_method();
      break;
#endif
    case TLSv1:
      LOG4CXX_DEBUG(logger_, "TLSv1 is used");
      method = is_server ? TLSv1_server_method() : TLSv1_client_method();
      break;
    case TLSv1_1:
      LOG4CXX_DEBUG(logger_, "TLSv1_1 is used");
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
      LOG4CXX_DEBUG(logger_, "TLSv1_2 is used");
#if OPENSSL_VERSION_NUMBER < TLS1_1_MINIMAL_VERSION
      LOG4CXX_WARN(
          logger_,
          "OpenSSL has no TLSv1.2 with version lower 1.0.1, set TLSv1.0");
      method = is_server ? TLSv1_server_method() : TLSv1_client_method();
#else
      method = is_server ? TLSv1_2_server_method() : TLSv1_2_client_method();
#endif
      break;
    case DTLSv1:
      LOG4CXX_DEBUG(logger_, "DTLSv1 is used");
      method = is_server ? DTLSv1_server_method() : DTLSv1_client_method();
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

  SaveCertificateData(get_settings().certificate_data());

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

  LOG4CXX_DEBUG(logger_, "Setting up module certificate and private key");

  X509* module_certificate = LoadModuleCertificateFromFile();
  utils::ScopeGuard certificate_guard =
      utils::MakeGuard(X509_free, module_certificate);
  UNUSED(certificate_guard);

  EVP_PKEY* module_key = LoadModulePrivateKeyFromFile();
  utils::ScopeGuard key_guard = utils::MakeGuard(EVP_PKEY_free, module_key);
  UNUSED(key_guard);

  if (!UpdateModuleCertificateData(module_certificate, module_key)) {
    LOG4CXX_WARN(logger_, "Failed to update module key and certificate");
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

  if (!SaveCertificateData(data)) {
    LOG4CXX_ERROR(logger_, "Failed to save certificate data");
    return false;
  }

  X509* module_certificate = LoadModuleCertificateFromFile();
  EVP_PKEY* module_key = LoadModulePrivateKeyFromFile();

  utils::ScopeGuard certificate_guard =
      utils::MakeGuard(X509_free, module_certificate);
  UNUSED(certificate_guard);

  utils::ScopeGuard key_guard = utils::MakeGuard(EVP_PKEY_free, module_key);
  UNUSED(key_guard);

  return UpdateModuleCertificateData(module_certificate, module_key);
}

SSLContext* CryptoManagerImpl::CreateSSLContext() {
  if (NULL == context_) {
    return NULL;
  }

  SSL* conn = SSL_new(context_);
  if (NULL == conn) {
    return NULL;
  }

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

bool CryptoManagerImpl::SaveCertificateData(const std::string& cert_data) {
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

  asn1_time_to_tm(X509_get_notAfter(cert));

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

  utils::ScopeGuard key_guard = utils::MakeGuard(EVP_PKEY_free, pkey);
  UNUSED(key_guard);

  return SaveModuleCertificateToFile(cert) && SaveModuleKeyToFile(pkey);
}

int CryptoManagerImpl::pull_number_from_buf(char* buf, int* idx) {
  if (!idx) {
    return 0;
  }
  const int val = ((buf[*idx] - '0') * 10) + buf[(*idx) + 1] - '0';
  *idx = *idx + 2;
  return val;
}

void CryptoManagerImpl::asn1_time_to_tm(ASN1_TIME* time) {
  char* buf = (char*)time->data;
  int index = 0;
  const int year = pull_number_from_buf(buf, &index);
  if (V_ASN1_GENERALIZEDTIME == time->type) {
    expiration_time_.tm_year =
        (year * 100 - 1900) + pull_number_from_buf(buf, &index);
  } else {
    expiration_time_.tm_year = year < 50 ? year + 100 : year;
  }

  const int mon = pull_number_from_buf(buf, &index);
  const int day = pull_number_from_buf(buf, &index);
  const int hour = pull_number_from_buf(buf, &index);
  const int mn = pull_number_from_buf(buf, &index);

  expiration_time_.tm_mon = mon - 1;
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
    const int sec = pull_number_from_buf(buf, &index);
    expiration_time_.tm_sec = sec;
  }
}

void CryptoManagerImpl::InitCertExpTime() {
  strptime("1 Jan 1970 00:00:00", "%d %b %Y %H:%M:%S", &expiration_time_);
}

bool CryptoManagerImpl::UpdateModuleCertificateData(X509* certificate,
                                                    EVP_PKEY* key) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (certificate) {
    if (!SSL_CTX_use_certificate(context_, certificate)) {
      LOG4CXX_WARN(logger_, "Could not use certificate: " << LastError());
      return false;
    }
  }

  if (key) {
    if (!SSL_CTX_use_PrivateKey(context_, key)) {
      LOG4CXX_ERROR(logger_, "Could not use key: " << LastError());
      return false;
    }

    if (!SSL_CTX_check_private_key(context_)) {
      LOG4CXX_ERROR(logger_, "Private key is invalid: " << LastError());
      return false;
    }
  }

  LOG4CXX_DEBUG(logger_, "Certificate and key are successfully updated");
  return true;
}

X509* CryptoManagerImpl::LoadModuleCertificateFromFile() {
  LOG4CXX_AUTO_TRACE(logger_);

  const std::string cert_path = get_settings().module_cert_path();
  BIO* bio_cert = BIO_new_file(cert_path.c_str(), "r");
  if (!bio_cert) {
    LOG4CXX_WARN(logger_,
                 "Failed to open " << cert_path << " file: " << LastError());
    return NULL;
  }

  utils::ScopeGuard bio_guard = utils::MakeGuard(BIO_free, bio_cert);
  UNUSED(bio_guard);

  X509* module_certificate = NULL;
  if (!PEM_read_bio_X509(bio_cert, &module_certificate, NULL, NULL)) {
    LOG4CXX_ERROR(logger_,
                  "Failed to read certificate data from file: " << LastError());
    return NULL;
  }
  LOG4CXX_DEBUG(logger_,
                "Module certificate was loaded: " << module_certificate);

  return module_certificate;
}

EVP_PKEY* CryptoManagerImpl::LoadModulePrivateKeyFromFile() {
  LOG4CXX_AUTO_TRACE(logger_);

  const std::string key_path = get_settings().module_key_path();
  BIO* bio_key = BIO_new_file(key_path.c_str(), "r");
  if (!bio_key) {
    LOG4CXX_WARN(logger_,
                 "Failed to open " << key_path << " file: " << LastError());
    return NULL;
  }

  utils::ScopeGuard bio_guard = utils::MakeGuard(BIO_free, bio_key);
  UNUSED(bio_guard);

  EVP_PKEY* module_key = NULL;
  if (!PEM_read_bio_PrivateKey(bio_key, &module_key, NULL, NULL)) {
    LOG4CXX_ERROR(logger_,
                  "Failed to read private key data from file: " << LastError());
    return NULL;
  }
  LOG4CXX_DEBUG(logger_, "Module private key was loaded: " << module_key);

  return module_key;
}

bool CryptoManagerImpl::SaveModuleCertificateToFile(X509* certificate) const {
  LOG4CXX_AUTO_TRACE(logger_);

  if (NULL == certificate) {
    LOG4CXX_WARN(logger_, "Empty certificate. Saving will be skipped");
    return false;
  }

  const std::string cert_path = get_settings().module_cert_path();
  BIO* bio_cert = BIO_new_file(cert_path.c_str(), "w");
  if (NULL == bio_cert) {
    LOG4CXX_ERROR(logger_,
                  "Failed to open " << cert_path << " file: " << LastError());
    return false;
  }

  if (0 == PEM_write_bio_X509(bio_cert, certificate)) {
    LOG4CXX_ERROR(logger_,
                  "Failed to write certificate to file: " << LastError());
    return false;
  }

  return true;
}

bool CryptoManagerImpl::SaveModuleKeyToFile(EVP_PKEY* key) const {
  LOG4CXX_AUTO_TRACE(logger_);

  if (NULL == key) {
    LOG4CXX_WARN(logger_, "Empty private key. Saving will be skipped");
    return false;
  }

  const std::string key_path = get_settings().module_key_path();
  BIO* bio_key = BIO_new_file(key_path.c_str(), "w");
  if (NULL == bio_key) {
    LOG4CXX_ERROR(logger_,
                  "Failed to open " << key_path << " file: " << LastError());
    return false;
  }

  if (0 == PEM_write_bio_PrivateKey(bio_key, key, NULL, NULL, 0, NULL, NULL)) {
    LOG4CXX_ERROR(logger_, "Failed to write key to file: " << LastError());
    return false;
  }

  return true;
}

}  // namespace security_manager
