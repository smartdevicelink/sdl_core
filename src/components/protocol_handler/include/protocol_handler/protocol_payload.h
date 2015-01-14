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
#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PAYLOAD_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PAYLOAD_H_

#include <stdint.h>
#include <ostream>
#include <string>
#include <vector>

#include "protocol/rpc_type.h"

namespace utils {
class BitStream;
}

namespace protocol_handler {

// Applink Protocolv5 4.1.2 Protocol Payload Binary header
struct ProtocolPayloadHeaderV2 {
  ProtocolPayloadHeaderV2()
    : rpc_type(kRpcTypeReserved),
      rpc_function_id(0), corellation_id(0), json_size(0) {}
  RpcType  rpc_type;
  uint32_t rpc_function_id;
  uint32_t corellation_id;
  uint32_t json_size;
};

// Applink Protocolv5 4.1.1 Protocol Message Payload
struct ProtocolPayloadV2 {
  ProtocolPayloadHeaderV2 header;
  std::string json;
  std::vector<uint8_t> data;
};

// Procedures that extract and validate defined protocol structures from
// a bit stream.
// If error during parsing is detected, bit stream is marked as invalid
void Extract(utils::BitStream *bs, ProtocolPayloadHeaderV2 *headerv2);
void Extract(utils::BitStream *bs, ProtocolPayloadV2 *payload, size_t payload_size);

std::ostream &operator<<(std::ostream &os, const ProtocolPayloadHeaderV2 &payload_header);
std::ostream &operator<<(std::ostream &os, const ProtocolPayloadV2 &payload);

//Add for tests
size_t ProtocolPayloadV2SizeBits();
} // namespace protocol_handler

#endif /* SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PAYLOAD_H_ */
