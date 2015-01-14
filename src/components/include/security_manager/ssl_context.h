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

#ifndef SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_SSL_CONTEXT_H_
#define SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_SSL_CONTEXT_H_

#include <cstddef>  // for size_t typedef
#include <string>

// TODO(EZamakhov): update brief info
/**
 * \class security_manager::SSLContext
 * \brief Class responsible for SSL connection establishing
 *        and data encryption and decryption within this connection
 *
 * \fn security_manager::SSLContext::DoHandshakeStep(
 *             const uint8_t*  in_data,  size_t in_data_size,
 *             const uint8_t** out_data, size_t *out_data_size)
 * \brief Performs SSL handshake
 * In order to establish an SSL connection it's
 * necessary to perform handshake process. During this process
 * client and server negotiate about encryption algorithms to use
 * and produce a session key.
 *
 * \param in_data        [in]   data sent by other side
 * \param in_data_size   [in]   size of data in \ref in_data buffer
 * \param out_data       [out]  response of SSL context if there is one. If not, equals NULL
 * \param out_data_size  [out]  length of response. On no response, equals 0
 */

namespace security_manager {
class SSLContext {
 public:
  enum HandshakeResult {
    Handshake_Result_Success      = 0x0,
    Handshake_Result_Fail         = 0x1,
    Handshake_Result_AbnormalFail = 0x2
  };
  virtual HandshakeResult StartHandshake(const uint8_t** const out_data,
                                         size_t *out_data_size) = 0;
  virtual HandshakeResult DoHandshakeStep(const uint8_t *const in_data,
                                          size_t in_data_size,
                                          const uint8_t** const out_data,
                                          size_t *out_data_size) = 0;
  virtual bool Encrypt(const uint8_t *const in_data,    size_t in_data_size,
                       const uint8_t ** const out_data, size_t *out_data_size) = 0;
  virtual bool Decrypt(const uint8_t *const in_data,    size_t in_data_size,
                       const uint8_t ** const out_data, size_t *out_data_size) = 0;
  virtual bool  IsInitCompleted() const = 0;
  virtual bool  IsHandshakePending() const = 0;
  virtual size_t get_max_block_size(size_t mtu) const = 0;
  virtual std::string LastError() const = 0;
  virtual ~SSLContext() { }
};
}  // namespace security_manager
#endif  // SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_SSL_CONTEXT_H_
