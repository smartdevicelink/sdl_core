/*
 * Copyright (c) 2020, Livio
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

/**
 * \file websocket_device.h
 * \brief WebSocketDevice class header file.
 */

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_DEVICE_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_DEVICE_H_

#include <boost/beast/websocket.hpp>
#include <string>
#include "transport_manager/transport_adapter/device.h"

namespace transport_manager {
namespace transport_adapter {

using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>
using protocol_type = boost::asio::basic_stream_socket<tcp>::protocol_type;

class WebSocketDevice : public Device {
 public:
  WebSocketDevice(const std::string& name, const DeviceUID& unique_device_id);

  virtual const std::string& GetHost() const;
  virtual const std::string& GetPort() const;
  virtual const std::string GetTarget() const;
  virtual void AddApplication(const ApplicationHandle& app_handle);

 protected:
  bool IsSameAs(const Device* other_device) const OVERRIDE;
  ApplicationList GetApplicationList() const OVERRIDE;

 private:
  std::string host_;
  std::string port_;
  bool is_secure_connect_;
  protocol_type protocol_;
  ApplicationList app_list_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_DEVICE_H_
