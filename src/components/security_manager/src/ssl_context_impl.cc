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

#include <assert.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <memory.h>

#include "utils/macro.h"

namespace security_manager {

CryptoManagerImpl::SSLContextImpl::SSLContextImpl(SSL *conn, Mode mode)
  : connection_(conn),
    bioIn_(BIO_new(BIO_s_mem())),
    bioOut_(BIO_new(BIO_s_mem())),
    bioFilter_(NULL),
    buffer_size_(1024), // TODO(DChmerev): Collect some statistics, determine the most appropriate value
    buffer_(new uint8_t[buffer_size_]),
    mode_(mode) {
  SSL_set_bio(connection_, bioIn_, bioOut_);
}

std::string LastError() {
  // TODO (DChmerev): add error on no key files
  const unsigned long error = ERR_get_error();
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

SSLContext::HandshakeResult CryptoManagerImpl::SSLContextImpl::
StartHandshake(const uint8_t** const out_data, size_t* out_data_size) {
  return DoHandshakeStep(NULL, 0, out_data, out_data_size);
}

SSLContext::HandshakeResult CryptoManagerImpl::SSLContextImpl::
DoHandshakeStep(const uint8_t*  const in_data,  size_t in_data_size,
                const uint8_t** const out_data, size_t* out_data_size) {
  *out_data = NULL;
  if (IsInitCompleted()) {
    return SSLContext::Handshake_Result_Success;
  }

  if (in_data && in_data_size) {
    int ret = BIO_write(bioIn_, in_data, in_data_size);
    if (ret <= 0) {
      return SSLContext::Handshake_Result_AbnormalFail;
    }
  }

  const int handshake_result = SSL_do_handshake(connection_);
  if (handshake_result == 1) {
    bioFilter_ = BIO_new(BIO_f_ssl());
    BIO_set_ssl(bioFilter_, connection_, BIO_NOCLOSE);
  } else if (handshake_result == 0) {
    return SSLContext::Handshake_Result_Fail;
  }

  const size_t pend = BIO_ctrl_pending(bioOut_);

  if (pend) {
    EnsureBufferSizeEnough(pend);

    const int read_count = BIO_read(bioOut_, buffer_, pend);
    if (read_count  == pend) {
      *out_data_size = read_count ;
      *out_data =  buffer_;
    } else {
      return SSLContext::Handshake_Result_AbnormalFail;
    }
  }

  return SSLContext::Handshake_Result_Success;
}

bool CryptoManagerImpl::SSLContextImpl::
Encrypt(const uint8_t *  const in_data,  size_t in_data_size,
        const uint8_t ** const out_data, size_t* out_data_size) {

  if (!SSL_is_init_finished(connection_) ||
      !in_data ||
      !in_data_size) {
    return false;
  }

  BIO_write(bioFilter_, in_data, in_data_size);
  const size_t len = BIO_ctrl_pending(bioOut_);

  EnsureBufferSizeEnough(len);
  const int read_size = BIO_read(bioOut_, buffer_, len);
  DCHECK(len == read_size);
  if (read_size < 0) {
    return false;
  }
  *out_data_size = read_size;
  *out_data = buffer_;

  return true;
}

bool CryptoManagerImpl::SSLContextImpl::
Decrypt(const uint8_t *  const in_data,  size_t in_data_size,
        const uint8_t ** const out_data, size_t* out_data_size) {

  if (!SSL_is_init_finished(connection_)) {
    return false;
  }

  if (!in_data || !in_data_size) {
    return false;
  }
  BIO_write(bioIn_, in_data, in_data_size);
  int len = BIO_ctrl_pending(bioFilter_);
  ptrdiff_t offset = 0;

  *out_data_size = 0;
  while (len) {
    EnsureBufferSizeEnough(len + offset);
    len = BIO_read(bioFilter_, buffer_ + offset, len);
    if (len < 0)
      return false;
    *out_data_size += len;
    offset += len;
    len = BIO_ctrl_pending(bioFilter_);
  }
  *out_data = buffer_;
  return true;
}

size_t CryptoManagerImpl::SSLContextImpl::
get_max_block_size(size_t mtu) const {
  // FIXME(EZamakhov): add correct logics for TLS1/1.2/SSL3
  // For SSL3.0 set temporary value 90, old TLS1.2 value is 29
  return mtu - 200;
/*
  const SSL_CIPHER *cipher = SSL_get_current_cipher(connection_);
*/
}

CryptoManagerImpl::SSLContextImpl::
~SSLContextImpl() {
  // TODO(EZamakhov): return destruction logics
  // SSL_shutdown(connection_);
  // SSL_free(connection_);
  // delete[] buffer_;
}

void CryptoManagerImpl::SSLContextImpl::EnsureBufferSizeEnough(size_t size) {
  if (buffer_size_ < size) {
    delete[] buffer_;
    buffer_ = new uint8_t[size];
    buffer_size_ = size;
  }
}

}  // namespace security_manager
