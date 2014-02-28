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
#include "secure_service_manager/crypto_manager_impl.h"

#include <assert.h>
#include <openssl/err.h>

#include <iostream>

namespace secure_service_manager {

CryptoManagerImpl::SSLContextImpl::SSLContextImpl(SSL *conn,
                                                  BIO *bioIn,
                                                  BIO *bioOut)
  : connection_(conn),
    bioIn_(bioIn),
    bioOut_(bioOut),
    bioFilter_(NULL) {
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

size_t CryptoManagerImpl::SSLContextImpl
::DoHandshake(char *in_data,  size_t in_data_size,
              char *out_data, size_t out_data_size) {

  if (SSL_is_init_finished(connection_)) {
    return 0;
  }

  if (in_data && (in_data_size > 0)) {
    int ret = BIO_write(bioIn_, in_data, in_data_size);

    ret = SSL_do_handshake(connection_);
    if (ret == 1) {
      bioFilter_ = BIO_new(BIO_f_ssl());
      BIO_set_ssl(bioFilter_, connection_, BIO_NOCLOSE);
    }

    int pen = BIO_pending(bioOut_);
    if (pen) {
      ret = BIO_read(bioOut_, out_data, out_data_size);
      if (ret > 0) {
        return ret;
      } else {
        return 0;
      }
    }
  }

  return 0;
}

size_t CryptoManagerImpl::SSLContextImpl
::Encrypt(char *in_data,  size_t in_data_size,
          char *out_data, size_t out_data_size) {

  if (!SSL_is_init_finished(connection_)) {
    return 0;
  }

  if (in_data && in_data_size) {
    BIO_write(bioFilter_, in_data, in_data_size);
  }

  size_t len = BIO_ctrl_pending(bioOut_);
  if (0 < len && len <= out_data_size) {
    return BIO_read(bioOut_, out_data, len);
  }

  return 0;
}

size_t CryptoManagerImpl::SSLContextImpl
::Decrypt(char *in_data,  size_t in_data_size,
          char *out_data, size_t out_data_size) {
  if (!SSL_is_init_finished(connection_)) {
    return 0;
  }

  if (in_data && in_data_size) {
    BIO_write(bioIn_, in_data, in_data_size);
  }

  size_t len = BIO_ctrl_pending(bioFilter_);
  if (0 < len && len <= out_data_size) {
    return BIO_read(bioFilter_, out_data, len);
  }

  return 0;
}

CryptoManagerImpl::SSLContextImpl::
~SSLContextImpl() {
  SSL_shutdown(connection_);
}

}  // namespace secure_service_manager
