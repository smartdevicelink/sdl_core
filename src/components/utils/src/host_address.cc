/*
 * Copyright (c) 2016, Ford Motor Company
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
#include "utils/host_address.h"
#include "utils/macro.h"

#if defined(OS_WINDOWS)
#include "utils/winhdr.h"
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace {

uint32_t ToInetAddress(const utils::SpecialAddress::Type address) {
  switch (address) {
    case utils::SpecialAddress::Any:
      return htonl(INADDR_ANY);
    case utils::SpecialAddress::LoopBack:
      return htonl(INADDR_LOOPBACK);
    default:
      DCHECK_OR_RETURN(!"Unsupported special address", htonl(INADDR_ANY));
  }
}

}  // namespace

utils::HostAddress::HostAddress() : ip4_(ToInetAddress(SpecialAddress::Any)) {}

utils::HostAddress::HostAddress(const utils::SpecialAddress::Type address)
    : ip4_(ToInetAddress(address)) {}

utils::HostAddress::HostAddress(const std::string& ip4_address)
    : ip4_(inet_addr(ip4_address.c_str())) {}

utils::HostAddress::HostAddress(const uint32_t ip4_address,
                                const bool is_host_byte_order)
    : ip4_(is_host_byte_order ? htonl(ip4_address) : ip4_address) {}

uint32_t utils::HostAddress::ToIp4Address(const bool is_host_byte_order) const {
  return is_host_byte_order ? ntohl(ip4_) : ip4_;
}

std::string utils::HostAddress::ToString() const {
  struct in_addr address;
  address.s_addr = ip4_;
  return inet_ntoa(address);
}

bool utils::HostAddress::operator==(const HostAddress& address) const {
  return ip4_ == address.ip4_;
}

bool utils::HostAddress::operator==(const SpecialAddress::Type address) const {
  return ip4_ == ToInetAddress(address);
}

bool utils::HostAddress::operator!=(const HostAddress& address) const {
  return !operator==(address);
}

bool utils::HostAddress::operator!=(const SpecialAddress::Type address) const {
  return !operator==(address);
}
