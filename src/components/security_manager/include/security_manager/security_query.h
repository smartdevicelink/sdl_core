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

#ifndef security_query_H
#define security_query_H

#include <cstddef>
#include <stdint.h>
#include "utils/shared_ptr.h"

namespace security_manager {

class SecurityQuery {
public:
  //size 1 byte
  enum QueryType {
    REQUEST      = 0x00,
    RESPONSE     = 0x10,
    NOTIFICATION = 0x20,
    INVALID_QUERY_TYPE = 0xFF
    };
  //size 3 bytes
  enum QueryId {
    PROTECT_SERVICE_REQUEST  = 0x1,
    PROTECT_SERVICE_RESPONSE = 0x2,
    SEND_HANDSHAKE_DATA      = 0x3,
    SEND_INTERNAL_ERROR      = 0xFF,
    INVALID_QUERY_ID         = 0xFFFFFF
  };
  //size 1 byte
  enum ProtectServiceResult {
    SUCCESS = 0x1,
    PENDING = 0x2,     //Handshake in progress
    SERVICE_ALREADY_PROTECTED = 0x3,
    SERVICE_NOT_FOUND = 0x4,
    INTERNAL_ERROR = 0xFF
  };
  //size 1 byte
  enum InternalErrors {
    ERROR_INVALID_QUERY_SIZE = 0x1, //low size of message
    ERROR_INVALID_QUERY_ID = 0x2,   //unknown query id
    ERROR_NULL_DATA = 0x3,          //Received query with null data
    ERROR_INVALID_SERVICE_TYPE = 0x4,//For unknown or forbidden for encryption service
    ERROR_PROTECTION_NOT_REQUESTED = 0x5, //Got handshake without ProtectServiceRequest
    ERROR_SSL_INVALID_DATA = 0xF0,
    ERROR_OTHER_INTERNAL_ERROR = 0xFF
    };

  struct QueryHeader {
    QueryHeader(uint8_t queryType, uint32_t queryId, uint32_t seqNumber);
    uint32_t query_type:8;
    uint32_t query_id:24;  // API function identifier
    uint32_t seq_number;   // request sequential number
    uint32_t reserved;
  };

  SecurityQuery();
  explicit SecurityQuery(const QueryHeader& header,
                              const uint32_t connection_key);
  ~SecurityQuery();
  bool Parse(const uint8_t * const binary_data, const size_t bin_data_size);
  void setData(const uint8_t * const binary_data, const size_t bin_data_size);
  void setConnectionKey(const uint32_t connection_key);

  void setHeader(const QueryHeader& header);
  const QueryHeader& getHeader() const;
  const uint8_t* const getData() const;
  size_t const getDataSize() const;
  int32_t getConnectionKey() const;
private:
  QueryHeader header_;
  int32_t connection_key_;
  uint8_t* data_;
  size_t data_size_;
};
typedef utils::SharedPtr<SecurityQuery> SecurityQueryPtr;
}
#endif // security_query_H
