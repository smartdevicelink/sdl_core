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

#include "protocol/rpc_type.h"

#include "utils/logger.h"
#include "utils/macro.h"

namespace protocol_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ProtocolHandler")

namespace {
bool IsSupported(RpcType rpc_type) {
  switch (rpc_type) {
    case kRpcTypeRequest:
    case kRpcTypeResponse:
    case kRpcTypeNotification:
      return true;
    default:
      return false;
  }
}
}  // namespace

RpcType RpcTypeFromByte(uint8_t byte) {
  RpcType type = RpcType(byte);
  bool supported_type = IsSupported(type);
  if (!supported_type) {
    LOG4CXX_INFO(logger_, "Invalid service type: " << int32_t(byte));
  }

  return supported_type ? type : kRpcTypeReserved;
}

uint8_t RpcTypeToByte(RpcType type) {
  DCHECK(IsSupported(type));
  return uint8_t(type);
}

const char *RpcTypeToString(RpcType type) {
  switch (type) {
    case kRpcTypeRequest:
      return "kRpcTypeRequest";
    case kRpcTypeResponse:
      return "kRpcTypeResponse";
    case kRpcTypeNotification:
      return "kRpcTypeNotification";
    default:
      return "kRpcTypeReserved";
  }
}

std::ostream &operator<<(std::ostream &os, RpcType rpc_type) {
  return os << RpcTypeToString(rpc_type);
}
}  // namespace protocol_handler
