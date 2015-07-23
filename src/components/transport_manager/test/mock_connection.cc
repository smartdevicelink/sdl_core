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

#include <cstring>

#include <sys/socket.h>
#include <sys/un.h>
#include <sstream>
#include "transport_manager/common.h"
#include "include/mock_connection.h"

#include <algorithm>

#include "include/mock_transport_adapter.h"

using ::transport_manager::transport_adapter::TransportAdapterController;

namespace test {
namespace components {
namespace transport_manager {

MockConnection::MockConnection(const ::transport_manager::DeviceUID& device_handle,
                               const ApplicationHandle& app_handle,
                               TransportAdapterController* controller)
    : ThreadedSocketConnection(device_handle, app_handle, controller) {
}

bool MockConnection::Establish(ConnectError **error) {
  int peer_sock = socket(AF_UNIX, SOCK_STREAM, 0);
  sockaddr_un my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  std::ostringstream iss;
  iss << "mockDevice" << device_handle() << "-" << application_handle();
  strcpy(my_addr.sun_path, iss.str().c_str());
  my_addr.sun_family = AF_UNIX;
  int res = ::connect(peer_sock, reinterpret_cast<sockaddr*>(&my_addr),
                      sizeof(my_addr));
  if (res != -1) {
    set_socket(peer_sock);
    return true;
  }
  *error = new ConnectError();
  return false;
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test

