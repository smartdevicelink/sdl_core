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

#include "vr_module/vr_proxy.h"

#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "vr_module/channel.h"
#include "vr_module/socket_channel.h"
#include "vr_module/vr_proxy_listener.h"

namespace vr_module {

static const int kHeaderSize = 4;

CREATE_LOGGERPTR_GLOBAL(logger_, "VRModule")

class Receiver : public threads::ThreadDelegate {
 public:
  explicit Receiver(VRProxy *parent)
      : parent_(parent),
        stop_(false) {
  }
  virtual void threadMain() {
    LOG4CXX_AUTO_TRACE(logger_);
    if (!parent_->channel_->Start()) {
      LOG4CXX_ERROR(logger_, "Could not start channel");
      return;
    }
    while (!stop_) {
      parent_->Receive();
    }
  }
  virtual void exitThreadMain() {
    LOG4CXX_AUTO_TRACE(logger_);
    stop_ = true;
    parent_->channel_->Stop();
  }

 private:
  VRProxy *parent_;
  volatile bool stop_;
};

VRProxy::VRProxy(VRProxyListener *listener)
    : listener_(listener),
      incoming_("VrIncoming", this),
      channel_(0),
      channel_thread_(0) {
  channel_ = new SocketChannel();
  StartChannel();
}

VRProxy::VRProxy(VRProxyListener* listener, Channel* channel)
    : listener_(listener),
      incoming_("VrIncoming", this),
      channel_(channel),
      channel_thread_(0) {
  StartChannel();
}

void VRProxy::StartChannel() {
  LOG4CXX_AUTO_TRACE(logger_);
  channel_thread_ = threads::CreateThread("ChannelReceiver",
                                          new Receiver(this));
  channel_thread_->start();
}

VRProxy::~VRProxy() {
  LOG4CXX_AUTO_TRACE(logger_);
  channel_thread_->join();
  delete channel_thread_->delegate();
  threads::DeleteThread(channel_thread_);
  delete channel_;
}

void VRProxy::OnReceived(const vr_hmi_api::Message& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  incoming_.PostMessage(message);
}

void VRProxy::Handle(vr_hmi_api::Message message) {
  LOG4CXX_AUTO_TRACE(logger_);
  listener_->OnReceived(message);
}

std::string VRProxy::SizeToString(int32_t value) {
  LOG4CXX_AUTO_TRACE(logger_);
  unsigned char result[kHeaderSize] = { 0 };
  result[0] = (value & 0x000000ff);
  result[1] = (value & 0x0000ff00) >> 8;
  result[2] = (value & 0x00ff0000) >> 16;
  result[3] = (value & 0xff000000) >> 24;
  std::string size(result, result + kHeaderSize);
  DCHECK(size.size() == kHeaderSize)
  return size;
}

int32_t VRProxy::SizeFromString(const std::string& value) {
  if (value.size() != kHeaderSize) {
    LOG4CXX_WARN(logger_, "Incorrect length of header");
    return 0;
  }
  unsigned char result[kHeaderSize] = { 0 };
  memcpy(result, value.c_str(), kHeaderSize);
  int32_t size = int32_t(result[0]) + int32_t(result[1] << 8)
      + int32_t(result[2] << 16) + int32_t(result[3] << 24);
  return size;
}

bool VRProxy::Send(const vr_hmi_api::Message& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string data;
  bool serialized = message.SerializeToString(&data);
  if (!serialized) {
    LOG4CXX_WARN(logger_, "Could not serialize message");
    return false;
  }
  std::string buffer = SizeToString(data.size()) + data;
  DCHECK(buffer.size() == kHeaderSize + data.size())
  if (!channel_) {
    LOG4CXX_ERROR(logger_, "Connection to HMI(Applink) is not established");
    return false;
  }
  return channel_->Send(data);
}

void VRProxy::Receive() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string data;
  if (channel_->Receive(kHeaderSize, &data)) {
    int size = SizeFromString(data);
    if (channel_->Receive(size, &data)) {
      vr_hmi_api::Message message;
      bool parsed = message.ParseFromString(data);
      if (parsed) {
        OnReceived(message);
      } else {
        LOG4CXX_WARN(logger_, "Could not parse message");
      }
    } else {
      LOG4CXX_WARN(logger_, "Could not read message");
    }
  } else {
    LOG4CXX_WARN(logger_, "Could not read size of message");
  }
}

}  // namespace vr_module
