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

#include "utils/logger.h"
#include "utils/threads/pulse_thread_delegate.h"

namespace threads {

SDL_CREATE_LOG_VARIABLE("Utils")

PulseThreadDelegate::PulseThreadDelegate() : run_(false) {
  SDL_LOG_TRACE("Creating QNX channel");
  chid_ = ChannelCreate(0);
  if (chid_ == -1) {
    SDL_LOG_ERROR("Failed to create QNX channel");
    return;
  }
  SDL_LOG_DEBUG("Created QNX channel " << chid_);

  SDL_LOG_TRACE("Connecting to QNX channel " << chid_);
  coid_ = ConnectAttach(ND_LOCAL_NODE, 0, chid_, _NTO_SIDE_CHANNEL, 0);
  if (coid_ == -1) {
    SDL_LOG_ERROR("Failed to connect to QNX channel " << chid_);
    return;
  }
  SDL_LOG_DEBUG("Connected to QNX channel " << chid_);

  run_ = true;
}

void PulseThreadDelegate::threadMain() {
  if (!Init()) {
    SDL_LOG_ERROR("Failed to initialize thread for QNX channel " << chid_);
    return;
  }
  while (run_) {
    struct sigevent event;
    SIGEV_PULSE_INIT(&event, coid_, SIGEV_PULSE_PRIO_INHERIT, PULSE_CODE, 0);
    if (ArmEvent(&event)) {
      struct _pulse pulse;
      SDL_LOG_INFO("Waiting for pulse on QNX channel " << chid_);
      if (MsgReceivePulse(chid_, &pulse, sizeof(pulse), 0) != -1) {
        SDL_LOG_INFO("Received pulse on QNX channel " << chid_);
        switch (pulse.code) {
          case PULSE_CODE:
            OnPulse();
            break;
        }
      } else {
        if (run_) {
          SDL_LOG_WARN("Error occurred while waiting for pulse on QNX channel "
                       << chid_);
        } else {
          SDL_LOG_INFO("QNX channel " << chid_ << " is apparently destroyed");
        }
      }
    }
  }
  Finalize();
}

void PulseThreadDelegate::exitThreadMain() {
  run_ = false;

  SDL_LOG_TRACE("Disconnecting from QNX channel " << chid_);
  if (ConnectDetach(coid_) != -1) {
    SDL_LOG_DEBUG("Disconnected from QNX channel " << chid_);
  } else {
    SDL_LOG_WARN("Failed to disconnect from QNX channel " << chid_);
  }

  SDL_LOG_TRACE("Destroying QNX channel " << chid_);
  if (ChannelDestroy(chid_) != -1) {  // unblocks MsgReceivePulse()
    SDL_LOG_DEBUG("QNX channel " << chid_ << " destroyed");
  } else {
    SDL_LOG_WARN("Failed to destroy QNX channel " << chid_);
  }
}

}  // namespace threads
