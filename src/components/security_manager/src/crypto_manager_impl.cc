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
#include "config_profile/profile.h"
#include "security_manager/security_manager.h"

namespace security_manager {

log4cxx::LoggerPtr CryptoManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("CryptoManagerImpl"));

const char* CertificatePath = "CertificatePath";
const char* KeyPath         = "KeyPath";
// SSL mode could be SERVER or CLIENT
const char* SSLMode         = "SSLMode";
// Could be ALL ciphers or list of chosen
const char* CipherList      = "CipherList";
// Verify Mobile app certificate
const char* VerifyPeer      = "VerifyPeer";
// Terminate handshake if mobile app did not return a certificate
const char* FialOnNoCert    = "FialOnNoCert";
// do not ask for certificate again in case of a renegotiation
const char* VerifyClientOnce= "VerifyClientOnce";

CryptoManagerImpl::CryptoManagerImpl()
    : context_(NULL) {
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();
  SSL_library_init();
}

bool CryptoManagerImpl::Init() {
  context_ = SSL_CTX_new(SSLv23_server_method());

  std::string config_value;
  profile::Profile::instance()->ReadStringValue(
        &config_value, "mycert.pem", SecurityManager::ConfigSection(), CertificatePath);
  LOG4CXX_INFO(logger_, "Certificate path: " << config_value);
  if (!SSL_CTX_use_certificate_file(context_, config_value.c_str(), SSL_FILETYPE_PEM)) {
    LOG4CXX_ERROR(logger_, "Could not use certificate " << config_value);
    return false;
  }

  profile::Profile::instance()->ReadStringValue(
        &config_value, "mykey.pem", SecurityManager::ConfigSection(), KeyPath);
  LOG4CXX_INFO(logger_, "Key path: " << config_value);
  if (!SSL_CTX_use_PrivateKey_file(context_, config_value.c_str(), SSL_FILETYPE_PEM)) {
    LOG4CXX_ERROR(logger_, "Could not use key " << config_value);
    return false;
  }

  if (!SSL_CTX_check_private_key(context_)) {
    LOG4CXX_ERROR(logger_, "Could not use certificate " << config_value);
    return false;
  }

  profile::Profile::instance()->ReadStringValue(
        &config_value, "ALL", SecurityManager::ConfigSection(), CipherList);
  LOG4CXX_INFO(logger_, "Cipher list: " << config_value);
  if(!SSL_CTX_set_cipher_list(context_, config_value.c_str())) {
    LOG4CXX_ERROR(logger_, "Could not set cipher list: " << config_value);
    return false;
  }

  bool verify_peer;
  profile::Profile::instance()->ReadBoolValue(
        &verify_peer, false, SecurityManager::ConfigSection(), VerifyPeer);

  int verify_mode_id = SSL_VERIFY_NONE;
  if(verify_peer) {
    LOG4CXX_INFO(logger_, "Enable SSL verify peer.");
    verify_mode_id |= SSL_VERIFY_PEER;
    bool fail_on_no_cert, verify_client_once;

    profile::Profile::instance()->ReadBoolValue(
          &fail_on_no_cert, false, SecurityManager::ConfigSection(), FialOnNoCert);
    if(fail_on_no_cert) {
      verify_mode_id |= SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
      LOG4CXX_INFO(logger_, "Enable fail on no certificate.");
    }

    profile::Profile::instance()->ReadBoolValue(
          &verify_client_once, false, SecurityManager::ConfigSection(), VerifyClientOnce);
    if(verify_client_once) {
      verify_mode_id |= SSL_VERIFY_CLIENT_ONCE;
      LOG4CXX_INFO(logger_, "Enable verify client one.");
    }
    }
  LOG4CXX_INFO(logger_, "Final verify mode: " << verify_mode_id);
  SSL_CTX_set_verify(context_, verify_mode_id, NULL);

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

  SSL_set_accept_state(conn);
  // TODO (EZamakhov) : add return NULL pointer on no keys
  return new SSLContextImpl(conn);
}

void CryptoManagerImpl::ReleaseSSLContext(SSLContext* context) {
  delete context;
}

} // namespace security_manager
