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

#include <assert.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <memory.h>

namespace security_manager {

CryptoManagerImpl::SSLContextImpl::SSLContextImpl(SSL *conn)
  : connection_(conn),
    bioIn_(BIO_new(BIO_s_mem())),
    bioOut_(BIO_new(BIO_s_mem())),
    bioFilter_(NULL),
    buffer_size_(1024), // TODO: Collect some statistics, determine the most appropriate value
    buffer_(new char[buffer_size_]) {
  SSL_set_bio(connection_, bioIn_, bioOut_);
}

std::string LastError() {
  long error = ERR_get_error();
  const char * reason = ERR_reason_error_string(error);
  if (reason) {
    return reason;
  } else {
    return std::string();
  }
}
bool CryptoManagerImpl::SSLContextImpl::IsInitCompleted() const {
  return SSL_is_init_finished(connection_);
}

void* CryptoManagerImpl::SSLContextImpl::
DoHandshakeStep(const void* client_data,  size_t client_data_size,
                size_t* server_data_size) {
  if (IsInitCompleted()) {
    return NULL;
  }

  if (!client_data || !client_data_size) {
    return NULL;
  }

  int ret = BIO_write(bioIn_, client_data, client_data_size);
  if (ret <= 0) {
    return NULL;
  }
  ret = SSL_do_handshake(connection_);
  if (ret == 1) {
    bioFilter_ = BIO_new(BIO_f_ssl());
    BIO_set_ssl(bioFilter_, connection_, BIO_NOCLOSE);
  }

  int pen = BIO_pending(bioOut_);

  if (!pen) {
    return NULL;
  }

  ret = BIO_read(bioOut_, static_cast<char*>(buffer_), pen);
  if (ret > 0) {
    *server_data_size = ret;
    return buffer_;
  } else {
    return NULL;
  }
}

void* CryptoManagerImpl::SSLContextImpl::
Encrypt(const void* data,  size_t data_size,
        size_t* encrypted_data_size) {

  if (!SSL_is_init_finished(connection_)) {
    return NULL;
  }

  if (!data || !data_size) {
    return NULL;
  }
  BIO_write(bioFilter_, data, data_size);
  int len = BIO_ctrl_pending(bioOut_);
  len = BIO_read(bioOut_, buffer_, len);
  if (len < 0)
    return NULL;
  *encrypted_data_size = len;
  return buffer_;
}

void* CryptoManagerImpl::SSLContextImpl::
Decrypt(const void* encrypted_data,  size_t encrypted_data_size,
        size_t* data_size) {

  if (!SSL_is_init_finished(connection_)) {
    return NULL;
  }

  if (!encrypted_data || !encrypted_data_size) {
    return NULL;
  }
  BIO_write(bioIn_, encrypted_data, encrypted_data_size);
  int len = BIO_ctrl_pending(bioFilter_);
  len = BIO_read(bioFilter_, buffer_, len);
  if (len < 0)
    return NULL;
  *data_size = len;
  return buffer_;
}

CryptoManagerImpl::SSLContextImpl::
~SSLContextImpl() {
  SSL_shutdown(connection_);
  SSL_free(connection_);
  delete[] buffer_;
}

}  // namespace security_manager
