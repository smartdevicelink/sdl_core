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

#ifndef CRYPTO_MANAGER_IMPL_H_
#define CRYPTO_MANAGER_IMPL_H_

#include <stdint.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>

#include "security_manager/crypto_manager.h"
#include "security_manager/ssl_context.h"
#include "utils/logger.h"
#include "utils/macro.h"

namespace security_manager {
  class CryptoManagerImpl : public CryptoManager {
   private:

    class SSLContextImpl : public SSLContext {
     public:
      SSLContextImpl(SSL *conn, Mode mode);
      virtual HandshakeResult StartHandshake(const uint8_t** const out_data, size_t* out_data_size);
      virtual HandshakeResult DoHandshakeStep(const uint8_t* const in_data,  size_t in_data_size,
                                              const uint8_t** const out_data, size_t* out_data_size);
      virtual bool Encrypt(const uint8_t * const in_data,  size_t in_data_size,
                            const uint8_t ** const out_data, size_t* out_data_size);
      virtual bool Decrypt(const uint8_t * const in_data,  size_t in_data_size,
                            const uint8_t ** const out_data, size_t* out_data_size);
      virtual bool  IsInitCompleted() const;
      virtual size_t get_max_block_size(size_t mtu) const;
      virtual ~SSLContextImpl();
     private:
      DISALLOW_COPY_AND_ASSIGN(SSLContextImpl);
      void EnsureBufferSizeEnough(size_t size);
      SSL *connection_;
      BIO *bioIn_;
      BIO *bioOut_;
      BIO *bioFilter_;
      size_t buffer_size_;
      uint8_t *buffer_;
      Mode mode_;
    };

   public:
    CryptoManagerImpl();
    virtual bool Init(Mode mode,
                      Protocol protocol,
                      const std::string& cert_filename,
                      const std::string& key_filename,
                      const std::string& ciphers_list,
                      bool verify_peer);
    virtual void Finish();
    virtual SSLContext *CreateSSLContext();
    virtual void ReleaseSSLContext(SSLContext* context);
   private:
    DISALLOW_COPY_AND_ASSIGN(CryptoManagerImpl);
    SSL_CTX *context_;
    Mode mode_;
    static ::log4cxx::LoggerPtr logger_;
    static int instance_count_;
  };
} // namespace security_manager

#endif /* CRYPTO_MANAGER_IMPL_H_ */
