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

#include "security_manager/security_query.h"
#include <cstring>
#include "utils/macro.h"

using namespace security_manager;

//TODO add info
inline uint32_t getUInt32Value(const uint8_t* const data){
  DCHECK(data);
  return
      data[0] << 24 | data[1] << 16 |
      data[2] <<  8 | data[3];
}
inline uint32_t getUInt24Value(const uint8_t* const data){
  DCHECK(data);
  return
      data[0] << 16 | data[1] <<  8 | data[2];
}

SecuityQuery::QueryHeader::QueryHeader(
    uint8_t queryType, uint32_t queryId, uint32_t seqNumber) :
  query_type(queryType), query_id(queryId), seq_number(seqNumber), reserved(0)  {
}

SecuityQuery::SecuityQuery() :
  header_(INVALID_QUERY_TYPE, INVALID_QUERY_ID, 0), connection_key_(0), data_(NULL) {
  }

SecuityQuery::SecuityQuery(
    const SecuityQuery::QueryHeader &header,
    const uint32_t connection_key) :
  header_(header), connection_key_(connection_key), data_(NULL) {
  }

SecuityQuery::~SecuityQuery() {
  delete data_;
}
bool SecuityQuery::Parse(const uint8_t * const binary_data,
                                 const size_t bin_data_size) {
  if(bin_data_size < sizeof(QueryHeader)) {
      return false;
    }
  const uint8_t guery_type = binary_data[0];
  switch (guery_type) {
    case REQUEST:
        header_.query_type = REQUEST;
      break;
    case RESPONSE:
        header_.query_type = RESPONSE;
      break;
    case NOTIFICATION:
        header_.query_type = NOTIFICATION;
      break;
    default:
      header_.query_type = INVALID_QUERY_TYPE;
      //TODO(EZ): check?
      break;
    }
  const uint8_t query_id = getUInt24Value(binary_data+1);
  switch (query_id) {
    case PROTECT_SERVICE_REQUEST:
      header_.query_id = PROTECT_SERVICE_REQUEST;
      break;
    case PROTECT_SERVICE_RESPONSE:
      header_.query_id = PROTECT_SERVICE_RESPONSE;
      break;
    case SEND_HANDSHAKE_DATA:
      header_.query_id = SEND_HANDSHAKE_DATA;
      break;
    case SEND_INTERNAL_ERROR:
      header_.query_id = SEND_INTERNAL_ERROR;
      break;
    default: // On wrong query id
      header_.query_id = INVALID_QUERY_ID;
      //TODO(EZ): check?
      break;
    }
  header_.seq_number = getUInt32Value(binary_data + 4);
  //skip data
  const int data_size = bin_data_size - sizeof(QueryHeader);
  if(data_size > 0) {
      delete data_;
      data_size_ = data_size;
      data_ = new uint8_t[data_size_];
      memcpy(data_, binary_data + sizeof(QueryHeader), data_size_);
    }
  return true;
  }

void SecuityQuery::setData(const uint8_t * const binary_data,
                                 const size_t bin_data_size) {
    DCHECK(binary_data); DCHECK(bin_data_size);
    delete data_;
    data_size_ = bin_data_size;
    data_ = new uint8_t[data_size_];
    memcpy(data_, binary_data, data_size_);
  }

void SecuityQuery::setConnectionKey(const uint32_t connection_key) {
    connection_key_ = connection_key;
  }

void SecuityQuery::setHeader(const SecuityQuery::QueryHeader &header) {
  header_ = header;
}

const SecuityQuery::QueryHeader &SecuityQuery::getHeader() const {
  return header_;
}

const uint8_t * const SecuityQuery::getData() const {
  return data_;
}

const size_t SecuityQuery::getDataSize() const {
  return data_size_;
}

int32_t SecuityQuery::getConnectionKey() const {
  return connection_key_;
}
