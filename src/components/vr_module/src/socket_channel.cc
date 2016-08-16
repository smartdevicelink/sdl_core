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

#include "vr_module/socket_channel.h"

#include "net/connected_socket_impl.h"
#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/scope_guard.h"

namespace vr_module {

CREATE_LOGGERPTR_GLOBAL(logger_, "VRModule")

SocketChannel::SocketChannel()
    : socket_(0) {
}

SocketChannel::~SocketChannel() {
  if (socket_) {
    socket_->close();
  }
  delete socket_;
}

bool SocketChannel::Start() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string address = "127.0.0.1";
  UInt32 port = 5431;
  socket_ = net::ConnectedSocketImpl::ConnectToHost(address.c_str(), port);
  return false;
}

bool SocketChannel::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN(socket_, false);
  socket_->shutdown();
  return true;
}

bool SocketChannel::Send(const std::string& data) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN(socket_, false);
  const size_t kChunkSize = 1000;
  const size_t total_size = data.size();
  size_t size_to_send = total_size < kChunkSize ? total_size : kChunkSize;
  UInt8* data_bytes = new UInt8[size_to_send];

  utils::ScopeGuard ppsdata_guard = utils::MakeGuard(
      utils::ArrayDeleter<UInt8*>, data_bytes);
  UNUSED(ppsdata_guard);

  size_t data_sent = 0;
  while (total_size > data_sent) {
    memcpy(data_bytes, data.c_str() + data_sent, size_to_send);
    size_t chunksent = socket_->send(data_bytes, size_to_send,
                                     net::NET_MSG_NOSIGNAL);
    data_sent += chunksent;
    memset(data_bytes, 0, size_to_send);
    if ((data_sent + kChunkSize) > total_size) {
      size_to_send = total_size - data_sent;
    }
  } LOG4CXX_INFO(logger_, "Sent " << data_sent);
  return true;
}

bool SocketChannel::Receive(size_t size, std::string* buffer) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN(socket_, false);
  const size_t kChunkSize = 1000;
  size_t to_recv = size < kChunkSize ? size : kChunkSize;

  size_t received = 0;
  UInt8* data_bytes = new UInt8[size];
  while (size > received) {
    ssize_t rev_size = socket_->recv(data_bytes, to_recv, 0);
    if (rev_size <= 0) {
      LOG4CXX_INFO(logger_, "recv error " << size);
      return false;
    }

    std::string partial;
    partial.assign(reinterpret_cast<char*>(data_bytes), rev_size);
    buffer->append(partial);
    memset(data_bytes, 0, rev_size);

    received += rev_size;
    if (received + kChunkSize > size) {
      to_recv = size - received;
    }
  }

  LOG4CXX_INFO(logger_, "Total received " << received);
  LOG4CXX_INFO(logger_, "String size " << buffer->size());
  return true;
}

}  // namespace vr_module
