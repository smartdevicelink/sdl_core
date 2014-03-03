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

#ifndef SRC_COMPONENTS_secure_service_manager_INCLUDE_secure_service_manager_SSL_CONTEXT_H
#define SRC_COMPONENTS_secure_service_manager_INCLUDE_secure_service_manager_SSL_CONTEXT_H

#include <cstddef> // for size_t typedef

#include <string>

/**
 * \class secure_service_manager::SSLContext
 * \brief Class responsible for SSL connection establishing
 *        and data encryption and decryption within this connection
 *
 * \fn secure_service_manager::SSLContext::DoHandshake(char *in_data,  size_t  in_data_size, char *out_data, size_t *out_data_size)
 * \brief Performs SSL handshake
 * In order to establish an SSL connection it's
 * necessary to perform handshake process. During this process
 * client and server negotiate about encryption algorithms to use
 * and produce a session key.
 *
 * \param data[in]      data sent by client
 * \param data_size[in] size of \ref in_data
 * \param out_data_size[out] length of server response
 * \return response of server
 */

namespace secure_service_manager {
class SSLContext {
 public:
  virtual void* DoHandshakeStep(const void* client_data,  size_t client_data_size,
                               size_t* server_data_size)=0;
  virtual void* Encrypt(const void* data,  size_t data_size,
                        size_t* encrypted_data_size)=0;
  virtual void* Decrypt(const void* encrypted_data,  size_t encrypted_data_size,
                        size_t* data_size)=0;
  virtual bool  IsInitCompleted() const = 0;
  virtual ~SSLContext() { }
};
} // namespace secure_service_manager
#endif // SRC_COMPONENTS_secure_service_manager_INCLUDE_secure_service_manager_SSL_CONTEXT_H
