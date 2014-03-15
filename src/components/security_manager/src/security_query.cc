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
#include "utils/byte_order.h"

using namespace security_manager;

SecurityQuery::QueryHeader::QueryHeader(
    uint8_t queryType, uint32_t queryId, uint32_t seqNumber) :
  query_type(queryType), query_id(queryId), seq_number(seqNumber), reserved(0) {
}

SecurityQuery::SecurityQuery() :
  header_(INVALID_QUERY_TYPE, INVALID_QUERY_ID, 0), connection_key_(0) {
}

SecurityQuery::SecurityQuery(
    const SecurityQuery::QueryHeader &header,
    const uint32_t connection_key) :
  header_(header), connection_key_(connection_key) {
}

bool SecurityQuery::Parse(const uint8_t * const binary_data,
                                 const size_t bin_data_size) {
  const size_t header_size = sizeof(QueryHeader);
  if(bin_data_size < header_size) {
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
      break;
  }
  // Convert to Little-Endian and clean high byte
  const uint32_t query_id = 0x00FFFFFF &
      BE_TO_LE32(*reinterpret_cast<const uint32_t*>(binary_data));
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
      break;
  }
  header_.seq_number = *reinterpret_cast<const uint32_t*>(binary_data + 4);
  if(bin_data_size > header_size) {
    // copy from end of header to end of binary data
    data_.assign(binary_data + header_size, binary_data + bin_data_size);
  }
  return true;
}

void SecurityQuery::set_data(const uint8_t * const binary_data,
                                 const size_t bin_data_size) {
    DCHECK(binary_data); DCHECK(bin_data_size);
    data_.assign(binary_data, binary_data + bin_data_size);
}

void SecurityQuery::set_connection_key(const uint32_t connection_key) {
  connection_key_ = connection_key;
}

void SecurityQuery::set_header(const SecurityQuery::QueryHeader &header) {
  header_ = header;
}

const SecurityQuery::QueryHeader &SecurityQuery::get_header() const {
  return header_;
}

const uint8_t * const SecurityQuery::get_data() const {
  return data_.data();
}

const size_t SecurityQuery::get_data_size() const {
  return data_.size();
}

int32_t SecurityQuery::get_connection_key() const {
  return connection_key_;
}
