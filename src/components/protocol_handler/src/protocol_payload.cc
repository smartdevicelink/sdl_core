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
#include "protocol_handler/protocol_payload.h"

#include <climits>

#include "utils/bitstream.h"
#include "utils/macro.h"

namespace {
// Protocol header field sizes
static const size_t kRpcTypeBits       = 4;
static const size_t kRpcFunctionIdBits = 32 - kRpcTypeBits;
static const size_t kCorrelationIdBits = 32;
static const size_t kJsonSizeBits      = 32;
static const size_t PayloadHeaderBits  = kRpcTypeBits +
                                         kRpcFunctionIdBits +
                                         kCorrelationIdBits +
                                         kJsonSizeBits;
}

namespace protocol_handler {

void Extract(utils::BitStream *bs, ProtocolPayloadHeaderV2 *headerv2) {
  DCHECK(bs && headerv2);
  if (headerv2 && bs && *bs) {
    uint8_t rpc_type;
    utils::Extract(bs, &rpc_type, kRpcTypeBits);
    headerv2->rpc_type = RpcTypeFromByte(rpc_type);
    if (headerv2->rpc_type == kRpcTypeReserved) {
      bs->MarkBad();
      return;
    }
    utils::Extract(bs, &headerv2->rpc_function_id, kRpcFunctionIdBits);
    utils::Extract(bs, &headerv2->correlation_id); // kCorrelationIdBits
    utils::Extract(bs, &headerv2->json_size);      // kJsonSizeBits
  }
}

void Extract(utils::BitStream *bs, ProtocolPayloadV2 *payload,
             size_t payload_size) {
  DCHECK(bs && payload);
  if (payload && bs && *bs) {
    Extract(bs, &payload->header);
    utils::Extract(bs, &payload->json, payload->header.json_size);
    size_t data_size = payload_size - payload->header.json_size -
        PayloadHeaderBits / CHAR_BIT;
    utils::Extract(bs, &payload->data, data_size);
  }
}

std::ostream &operator<<(std::ostream &os,
                         const ProtocolPayloadHeaderV2 &payload_header) {
  return os << "(ProtocolPayloadHeaderV2"     << "  rpc_type: "
            << payload_header.rpc_type        << ", rpc_function_id: "
            << payload_header.rpc_function_id << ", correlation_id: "
            << payload_header.correlation_id  << ", json_size: "
            << payload_header.json_size       << ")";
}

std::ostream &operator<<(std::ostream &os, const ProtocolPayloadV2 &payload) {
  return os << "(ProtocolPayloadV2" << "  header: " << payload.header
            << ", json (bytes): "   << payload.json.size() << ", data (bytes): "
            << payload.data.size()  << ")";
}

size_t ProtocolPayloadV2SizeBits() {
  return PayloadHeaderBits;
}

}  // namespace protocol_handler
