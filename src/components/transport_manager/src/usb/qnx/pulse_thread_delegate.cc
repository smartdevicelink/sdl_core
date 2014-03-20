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

#include <sys/netmgr.h>

#include "transport_manager/usb/qnx/pulse_thread_delegate.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

PulseThreadDelegate::PulseThreadDelegate() : run_(false) {
  LOG4CXX_TRACE(logger_, "Creating QNX channel");
  chid_ = ChannelCreate(0);
  if (chid_ != -1) {
    LOG4CXX_DEBUG(logger_, "Created QNX channel " << chid_);
  }
  else {
    LOG4CXX_ERROR(logger_, "Failed to create QNX channel");
    return;
  }

  LOG4CXX_TRACE(logger_, "Connecting to QNX channel " << chid_);
  coid_ = ConnectAttach(ND_LOCAL_NODE, 0, chid_, _NTO_SIDE_CHANNEL, 0);
  if (coid_ != -1) {
    LOG4CXX_DEBUG(logger_, "Connected to QNX channel " << chid_);
  }
  else {
    LOG4CXX_ERROR(logger_, "Failed to connect to QNX channel " << chid_);
    return;
  }

  run_ = true;
}

void PulseThreadDelegate::threadMain() {
  while (run_) {
    struct sigevent event;
    SIGEV_PULSE_INIT(&event, coid_, SIGEV_PULSE_PRIO_INHERIT, PULSE_CODE_EAP, 0);
    if (ArmEvent(&event)) {
      struct _pulse pulse;
      LOG4CXX_INFO(logger_, "Waiting for pulse on QNX channel " << chid_);
      if (MsgReceivePulse(chid_, &pulse, sizeof(pulse), 0) != -1) {
        LOG4CXX_INFO(logger_, "Received pulse on QNX channel " << chid_);
        switch (pulse.code) {
          case PULSE_CODE_EAP:
            OnPulse();
            break;
        }
      }
      else {
        LOG4CXX_WARN(logger_, "Error occured while waiting for pulse on QNX channel " << chid_);
      }
    }
  }
}

bool PulseThreadDelegate::exitThreadMain() {
  run_ = false;

  LOG4CXX_TRACE(logger_, "Disconnecting from QNX channel" << chid_);
  if (ConnectDetach(coid_) != -1) {
    LOG4CXX_DEBUG(logger_, "Disconnected from QNX channel " << chid_);
  }
  else {
    LOG4CXX_WARN(logger_, "Failed to disconnect from QNX channel " << chid_);
  }

  LOG4CXX_TRACE(logger_, "Destroying QNX channel " << chid_);
  if (ChannelDestroy(chid_) != -1) { // unblocks MsgReceivePulse()
    LOG4CXX_DEBUG(logger_, "QNX channel " << chid_ << " destroyed");
  }
  else {
    LOG4CXX_WARN(logger_, "Failed to destroy QNX channel " << chid_);
  }

  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager
