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

#include "hmi_message_handler/mqueue_adapter.h"
#include "hmi_message_handler/hmi_message_handler.h"
#include "utils/logger.h"

namespace hmi_message_handler {

const uint32_t kMqueueSize = 100;
const uint32_t kMqueueMessageSize = 65536;
const char* kSdlToHmiQueue = "/sdl_to_hmi";
const char* kHmiToSdlQueue = "/hmi_to_sdl";

CREATE_LOGGERPTR_GLOBAL(logger_, "HMIMessageHandler")

class ReceiverThreadDelegate : public threads::ThreadDelegate {
 public:
  ReceiverThreadDelegate(mqd_t mqueue_descriptor,
                         HMIMessageHandler* hmi_message_handler)
      : mqueue_descriptor_(mqueue_descriptor),
        hmi_message_handler_(hmi_message_handler) {}

 private:
  virtual void threadMain() {
    while (true) {
      static char buffer[kMqueueMessageSize];
      const ssize_t size =
          mq_receive(mqueue_descriptor_, buffer, kMqueueMessageSize, NULL);
      if (-1 == size) {
        LOG4CXX_ERROR(logger_, "Message queue receive failed, error " << errno);
        continue;
      }
      const std::string message_string(buffer, buffer + size);
      LOG4CXX_INFO(logger_, "Message: " << message_string);
      MessageSharedPointer message(new application_manager::Message(
          protocol_handler::MessagePriority::kDefault));
      message->set_json_message(message_string);
      message->set_protocol_version(application_manager::ProtocolVersion::kHMI);
      hmi_message_handler_->OnMessageReceived(message);
    }
  }

  const mqd_t mqueue_descriptor_;
  HMIMessageHandler* hmi_message_handler_;
};

MqueueAdapter::MqueueAdapter(HMIMessageHandler* hmi_message_handler)
    : HMIMessageAdapter(hmi_message_handler),
      sdl_to_hmi_mqueue_(-1),
      hmi_to_sdl_mqueue_(-1),
      receiver_thread_() {
  mq_attr mq_attributes;
  mq_attributes.mq_maxmsg = kMqueueSize;
  mq_attributes.mq_msgsize = kMqueueMessageSize;
  sdl_to_hmi_mqueue_ =
      mq_open(kSdlToHmiQueue, O_CREAT | O_RDWR, S_IRWXU, &mq_attributes);
  if (-1 == sdl_to_hmi_mqueue_) {
    LOG4CXX_ERROR(logger_, "Could not open message queue "
                              << kSdlToHmiQueue << ", error " << errno);
    return;
  }
  hmi_to_sdl_mqueue_ =
      mq_open(kHmiToSdlQueue, O_CREAT | O_RDWR, S_IRWXU, &mq_attributes);
  if (-1 == hmi_to_sdl_mqueue_) {
    LOG4CXX_ERROR(logger_, "Could not open message queue "
                              << kHmiToSdlQueue << ", error " << errno);
    return;
  }
  ReceiverThreadDelegate* receiver_thread_delegate =
      new ReceiverThreadDelegate(hmi_to_sdl_mqueue_, hmi_message_handler);
  receiver_thread_ =
      threads::CreateThread("MqueueAdapter", receiver_thread_delegate);
  receiver_thread_->start();
}

MqueueAdapter::~MqueueAdapter() {
  if (receiver_thread_) {
    receiver_thread_->stop();
    threads::DeleteThread(receiver_thread_);
  }
  if (-1 != hmi_to_sdl_mqueue_) mq_close(hmi_to_sdl_mqueue_);
  if (-1 != sdl_to_hmi_mqueue_) mq_close(sdl_to_hmi_mqueue_);
  mq_unlink(kHmiToSdlQueue);
  mq_unlink(kSdlToHmiQueue);
}

void MqueueAdapter::SendMessageToHMI(const MessageSharedPointer message) {
  LOG4CXX_TRACE_ENTER(logger_);

  if (-1 == sdl_to_hmi_mqueue_) {
    LOG4CXX_ERROR(logger_, "Message queue is not opened");
    return;
  }
  const std::string& json = message->json_message();
  if (json.size() > kMqueueMessageSize) {
    LOG4CXX_ERROR(logger_, "Message size " << json.size() << " is too big");
    return;
  }
  const int rc = mq_send(sdl_to_hmi_mqueue_, json.c_str(), json.size(), 0);
  if (0 != rc) {
    LOG4CXX_ERROR(logger_, "Could not send message, error " << errno);
    return;
  }

  LOG4CXX_TRACE_EXIT(logger_);
}

void MqueueAdapter::SubscribeTo() {
  // empty implementation of pure virtual method, actually it's not called
}

}  // namespace hmi_message_handler
