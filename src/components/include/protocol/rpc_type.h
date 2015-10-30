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
#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_RPC_TYPE_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_RPC_TYPE_H_

#include <stdint.h>

#include <ostream>

namespace protocol_handler {

// AppLink protocol 4.1.3 RpcType field
enum RpcType {
  kRpcTypeRequest      = 0x0,
  kRpcTypeResponse     = 0x1,
  kRpcTypeNotification = 0x2,
  kRpcTypeReserved
};

// Validate and map byte value to RPC type
RpcType RpcTypeFromByte(uint8_t byte);
// Check and convert RpcType to byte value ready to be transmitted
uint8_t RpcTypeToByte(RpcType type);

const char *RpcTypeToString(RpcType type);

std::ostream &operator<<(std::ostream &os, RpcType rpc_type);
}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_RPC_TYPE_H_
