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

#include "transport_manager/mme/mme_device_scanner.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

const char* MmeDeviceScanner::mme_name = "/dev/mme/default";

MmeDeviceScanner::MmeDeviceScanner(TransportAdapterController* controller) : initialised_(false) {
}

MmeDeviceScanner::~MmeDeviceScanner() {
}

TransportAdapter::Error MmeDeviceScanner::Init() {
  LOG4CXX_TRACE(logger_, "Connecting to " << mme_name);
  mme_hdl_ = mme_connect(mme_name, 0);
  if (mme_hdl_ != 0) {
    LOG4CXX_DEBUG(logger_, "Connected to " << mme_name);
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not connect to " << mme_name);
    return TransportAdapter::FAIL;
  }

  notify_thread_ = new threads::Thread("MME notifier", new NotifyThreadDelegate(mme_hdl_, this));
  notify_thread_->start();

  initialised_ = true;
  return TransportAdapter::OK;
}

TransportAdapter::Error MmeDeviceScanner::Scan() {
  return TransportAdapter::OK;
}

void MmeDeviceScanner::Terminate() {
  notify_thread_->stop();
  LOG4CXX_TRACE(logger_, "Disconnecting from " << mme_name);
  if (mme_disconnect(mme_hdl_) != -1) {
    LOG4CXX_DEBUG(logger_, "Disconnected from " << mme_name);
  }
  else {
    LOG4CXX_WARN(logger_, "Could not disconnect from " << mme_name);
  }
}

bool MmeDeviceScanner::IsInitialised() const {
  return initialised_;
}

void MmeDeviceScanner::OnDeviceArrived(uint64_t msid) {
}

void MmeDeviceScanner::OnDeviceLeft(uint64_t msid) {
}

MmeDeviceScanner::NotifyThreadDelegate::NotifyThreadDelegate(mme_hdl_t* mme_hdl, MmeDeviceScanner* parent) : parent_(parent), mme_hdl_(mme_hdl) {
}

bool MmeDeviceScanner::NotifyThreadDelegate::ArmEvent(sigevent* event) {
  LOG4CXX_TRACE(logger_, "Arming for MME event notification");
  if (mme_register_for_events(mme_hdl_, MME_EVENT_CLASS_SYNC, event) != -1) {
    LOG4CXX_DEBUG(logger_, "Successfully armed for MME event notification");
    return true;
  }
  else {
    LOG4CXX_WARN(logger_, "Could not arm for MME event notification");
    return false;
  }
}

void MmeDeviceScanner::NotifyThreadDelegate::OnPulse() {
  mme_event_t* mme_event;
  LOG4CXX_TRACE(logger_, "Getting MME event");
  if (mme_get_event(mme_hdl_, &mme_event) != -1) {
    LOG4CXX_DEBUG(logger_, "Parsing MME event");
    switch (mme_event->type) {
      case MME_EVENT_MS_STATECHANGE: {
        mme_ms_statechange_t* mme_ms_statechange = (mme_ms_statechange_t*) (mme_event->data);
        uint64_t msid = mme_ms_statechange->msid;
        uint32_t old_state = mme_ms_statechange->old_state;
        uint32_t new_state = mme_ms_statechange->new_state;
        LOG4CXX_DEBUG(logger_, "MME event: msid = " << msid << ", old_state = " << old_state << ", new_state = " << new_state);
        if ((0 == old_state) && (3 == new_state)) {  // XXX
          parent_->OnDeviceArrived(msid);
        }
        break;
      }
    }
  }
  else {
    LOG4CXX_WARN(logger_, "Could not get MME event");
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
