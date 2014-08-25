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

#include "transport_manager/aoa/aoa_client_listener.h"

#include <unistd.h>
#include <sys/iomsg.h>
#include <sys/pps.h>

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

AOAClientListener::AOAClientListener(TransportAdapterController* controller)
    : controller_(controller) {
}

TransportAdapter::Error AOAClientListener::Init() {
  return TransportAdapter::OK;

}

void AOAClientListener::Terminate() {

}

bool AOAClientListener::IsInitialised() const {
  return true;
}

TransportAdapter::Error AOAClientListener::StartListening() {
  return TransportAdapter::OK;
}

TransportAdapter::Error AOAClientListener::StopListening() {
  return TransportAdapter::OK;
}

AOAClientListener::PpsThreadDelegate::PpsThreadDelegate(AOAClientListener* parent)
    : parent_(parent),
      fd_(-1) {
}

bool AOAClientListener::PpsThreadDelegate::Init() {
  if ( (fd_ = open(PPS_PATH_ROOT PPS_PATH_ALL, O_RDONLY)) == -1 ) {
    LOG4CXX_ERROR(logger_, "Error opening file '" << PPS_PATH_ROOT PPS_PATH_ALL
                  << "': (" << strerror(errno) << ")");
    return false;
  }
  return true;
}

bool AOAClientListener::PpsThreadDelegate::ArmEvent(struct sigevent* event) {
  uint8_t buf[2048];
  while (ionotify(fd_, _NOTIFY_ACTION_POLLARM, _NOTIFY_COND_INPUT, &event)
      & _NOTIFY_COND_INPUT) {
    int size = read(fd_, buf, sizeof(buf));
    if (size > 0) {
      buf[size] = '\0';
      //parent_->process_ppsdata(buf, size);
    }
  }
  return true;
}

void AOAClientListener::PpsThreadDelegate::OnPulse() {
}

void AOAClientListener::PpsThreadDelegate::Finalize() {
  close(fd_);
  fd_ = -1;
}

}  // namespace transport_adapter
}  // namespace transport_manager

