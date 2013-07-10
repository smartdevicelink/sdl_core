/**
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

#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "config_profile/profile.h"
#include "./to_hmi_thread_impl.h"
#include "./from_hmi_thread_impl.h"

namespace hmi_message_handler {

log4cxx::LoggerPtr HMIMessageHandlerImpl::logger_   =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("HMIMessageHandler"));

HMIMessageHandlerImpl* HMIMessageHandlerImpl::instance() {
  static HMIMessageHandlerImpl instance;
  return &instance;
}

HMIMessageHandlerImpl::HMIMessageHandlerImpl()
  : observer_(NULL)
  , to_hmi_thread_(NULL)
  , from_hmi_thread_(NULL) {
  to_hmi_thread_ = new threads::Thread(
    "hmi_message_handler::ToHMIThreadImpl",
    new ToHMIThreadImpl(this));
  to_hmi_thread_->startWithOptions(
    threads::ThreadOptions(
      profile::Profile::instance()->thread_min_stach_size()));

  from_hmi_thread_ = new threads::Thread(
    "hmi_message_handler::FromHMIThreadImpl",
    new FromHMIThreadImpl(this));
  from_hmi_thread_->startWithOptions(
    threads::ThreadOptions(
      profile::Profile::instance()->thread_min_stach_size()));
}

HMIMessageHandlerImpl::~HMIMessageHandlerImpl() {
  LOG4CXX_INFO(logger_, "HMIMessageHandlerImpl::~HMIMessageHandlerImpl()");
  observer_ = NULL;
  message_adapters_.clear();

  if (to_hmi_thread_) {
    to_hmi_thread_->stop();
    delete to_hmi_thread_;
    to_hmi_thread_ = NULL;
  }

  if (from_hmi_thread_) {
    from_hmi_thread_->stop();
    delete from_hmi_thread_;
    from_hmi_thread_ = NULL;
  }
}

void HMIMessageHandlerImpl::onMessageReceived(
  MessageSharedPointer message) {
  LOG4CXX_INFO(logger_, "HMIMessageHandlerImpl::~onMessageReceived()");
  if (!observer_) {
    // TODO(PV): WARNING
    return;
  }
  messages_from_hmi_.push(message);
}

void HMIMessageHandlerImpl::sendMessageToHMI(
  MessageSharedPointer message) {
  LOG4CXX_INFO(logger_, "HMIMessageHandlerImpl::~sendMessageToHMI()");
  messages_to_hmi_.push(message);
}

void HMIMessageHandlerImpl::setMessageObserver(HMIMessageObserver* observer) {
  LOG4CXX_INFO(logger_, "HMIMessageHandlerImpl::~setMessageObserver()");
  observer_ = observer;
}

void HMIMessageHandlerImpl::onErrorSending(
  MessageSharedPointer message) {
  LOG4CXX_INFO(logger_, "HMIMessageHandlerImpl::~onErrorSending()");
  if (!observer_) {
    // TODO(PV): WARNING
    return;
  }
  observer_->onErrorSending(message);
}

void HMIMessageHandlerImpl::addHMIMessageAdapter(HMIMessageAdapter* adapter) {
  LOG4CXX_INFO(logger_, "HMIMessageHandlerImpl::~addHMIMessageAdapter()");
  message_adapters_.insert(adapter);
}

void HMIMessageHandlerImpl::removeHMIMessageAdapter(
  HMIMessageAdapter* adapter) {
  LOG4CXX_INFO(logger_, "HMIMessageHandlerImpl::~removeHMIMessageAdapter()");
  message_adapters_.erase(adapter);
}

}  //  namespace hmi_message_handler
