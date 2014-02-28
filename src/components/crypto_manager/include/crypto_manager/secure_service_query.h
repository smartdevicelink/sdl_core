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

#ifndef SECURE_SERVICE_QUERY_H
#define SECURE_SERVICE_QUERY_H

#include <cstddef>
//TODO: EZamakhov remove <cstdint> as C++11
#include <cstdint>
#include "utils/shared_ptr.h"

namespace crypto_manager {

enum SecureServiceQueryId {
  ProtectServiceRequest  = 0x1,
  ProtectServiceResponse = 0x2,
  SendHandshakeData = 0x3,
  InvalidSecureServiceQuery
};

enum ProtectServiceResult {
  SUCCESS = 0x1,
  PENDING = 0x2,     //Handshake in progress
  SERVICE_ALREADY_PROTECTED = 0x3,
  SERVICE_NOT_FOUND = 0x4,
  INTERNAL_ERROR = 0xFF
};

class SecureServiceQuery {
public:
  struct QueryHeader {
    QueryHeader() : query_id_(InvalidSecureServiceQuery), seq_number_(0){}
    uint8_t  query_id_;    // API function identifier
    uint32_t seq_number_;  // request sequential number
  };

  SecureServiceQuery();
  bool setData(const void* const message_data, const size_t message_data_size);
  ~SecureServiceQuery();
private:
  QueryHeader header_;
  void* data_;
};
typedef utils::SharedPtr<SecureServiceQuery> SecureServiceQueryPtr;
}
#endif // SECURE_SERVICE_QUERY_H
