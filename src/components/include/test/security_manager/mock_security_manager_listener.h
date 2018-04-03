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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SECURITY_MANAGER_LISTENER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SECURITY_MANAGER_LISTENER_H_

#include "gmock/gmock.h"
#include "security_manager/security_manager_listener.h"
#include "security_manager/ssl_context.h"

namespace test {
namespace components {
namespace security_manager_test {

class MockSecurityManagerListener
    : public ::security_manager::SecurityManagerListener {
 public:
  MOCK_METHOD2(OnHandshakeDone,
               bool(uint32_t connection_key,
                    ::security_manager::SSLContext::HandshakeResult result));
  MOCK_METHOD0(OnCertificateUpdateRequired, void());
  MOCK_CONST_METHOD1(GetPolicyCertificateData, bool(std::string& data));
  MOCK_METHOD0(OnHandshakeFailed, void());
};
}  // namespace security_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SECURITY_MANAGER_LISTENER_H_
