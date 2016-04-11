/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SSL_CONTEXT_H_
#define SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SSL_CONTEXT_H_

#include <string>

#include "gmock/gmock.h"
#include "security_manager/ssl_context.h"

namespace test {
namespace components {
namespace security_manager_test {

class MockSSLContext : public ::security_manager::SSLContext {
 public:
  MOCK_METHOD2(StartHandshake, HandshakeResult(const uint8_t** const out_data,
                                               size_t* out_data_size));
  MOCK_METHOD4(DoHandshakeStep, HandshakeResult(const uint8_t* const in_data,
                                                size_t in_data_size,
                                                const uint8_t** const out_data,
                                                size_t* out_data_size));
  MOCK_METHOD4(Encrypt,
               bool(const uint8_t* const in_data, size_t in_data_size,
                    const uint8_t** const out_data, size_t* out_data_size));
  MOCK_METHOD4(Decrypt,
               bool(const uint8_t* const in_data, size_t in_data_size,
                    const uint8_t** const out_data, size_t* out_data_size));
  MOCK_CONST_METHOD0(IsInitCompleted, bool());
  MOCK_CONST_METHOD0(IsHandshakePending, bool());
  MOCK_CONST_METHOD1(get_max_block_size, size_t(size_t mtu));
  MOCK_CONST_METHOD0(LastError, std::string());
  MOCK_METHOD0(ResetConnection, void());
  MOCK_METHOD1(SetHandshakeContext, void(const HandshakeContext& hsh_ctx));
};
}  // namespace security_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SSL_CONTEXT_H_
