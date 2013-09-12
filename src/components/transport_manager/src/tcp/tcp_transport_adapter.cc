/**
 * \file tcp_transport_adapter.cpp
 * \brief TcpTransportAdapter class source file.
 *
 * Copyright (c) 2013, Ford Motor Company
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

#include <memory.h>
#include <signal.h>
#include <errno.h>

#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "transport_manager/tcp/tcp_client_listener.h"
#include "transport_manager/tcp/tcp_connection_factory.h"
#include "transport_manager/tcp/dnssd_service_browser.h"

namespace transport_manager {
namespace transport_adapter {

TcpTransportAdapter::TcpTransportAdapter()
    : TransportAdapterImpl(new DnssdServiceBrowser(this), new TcpConnectionFactory(this),
                        new TcpClientListener(this, default_port)) {
}

TcpTransportAdapter::~TcpTransportAdapter() {
}

DeviceType TcpTransportAdapter::GetDeviceType() const {
  return "sdl-tcp";
}

}  // namespace transport_adapter
}  // namespace transport_manager

