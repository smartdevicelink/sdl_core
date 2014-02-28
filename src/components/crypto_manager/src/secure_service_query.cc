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

#include "crypto_manager/secure_service_query.h"
#include <cstring>
#include "utils/macro.h"

using namespace crypto_manager;

//TODO add info
inline uint32_t getUInt8Value(const uint8_t* const data){
  DCHECK(data);
  return
      data[0] << 24 | data[1] << 16 |
      data[2] <<  8 | data[3];
}

SecureServiceQuery::SecureServiceQuery() : header_(), data_(NULL) {
}

bool SecureServiceQuery::setData(const void * const message_data,
                                 const size_t message_data_size) {
  DCHECK(message_data);
  const size_t header_size = sizeof(QueryHeader);
  if(message_data_size < header_size) {
      return false;
    }

  const uint8_t* const binary_data =
      static_cast<const uint8_t* const>(message_data);
  SecureServiceQuery::QueryHeader header;
  header.query_id_ = binary_data[0];
  header.seq_number_ = getUInt8Value(binary_data + 1);
  header.seq_number_ = getUInt8Value(binary_data + 5);
  const int data_size = message_data_size - header_size;
  if(data_size > 0) {
      data_ = new uint8_t[data_size];
      memcpy(data_, message_data + header_size, data_size);
    }
  return true;
}

SecureServiceQuery::~SecureServiceQuery() {
  delete data_;
  data_=NULL;
}

