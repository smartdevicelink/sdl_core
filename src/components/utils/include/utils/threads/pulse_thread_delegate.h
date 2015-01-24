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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_PULSE_THREAD_DELEGATE_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_PULSE_THREAD_DELEGATE_H_

#include <sys/neutrino.h>

#include "utils/threads/thread_delegate.h"

namespace threads {

/**
 * @brief This ThreadDelegate derivative is designed
 * to implement threads waiting for QNX Pulse messages
 * When constucted, an instance of this class creates QNX channel and connects to it
 * In exitThreadMain() channel is disconnected and destroyed
 * In threadMain() endless loop event is armed via pure virtual method ArmEvent()
 * and thread blocks on MsgReceivePulse() waiting for Pulse
 * When Pulse comes, OnPulse() pure virtual method is invoked
 * Subclassed must implement ArmEvent() for events of interest
 * and OnPulse() for reaction on such events
 */
class PulseThreadDelegate : public ThreadDelegate {
 public:
/**
  * @brief default constructor
  */
  PulseThreadDelegate();
  virtual void threadMain();
  virtual void exitThreadMain();

 protected:
/**
  * @brief This method is to be implemented to arm events of interest
  * @param event pointer to structure sigevent
  * @return If this method returns true, thread is blocked on MsgReceivePulse() waiting for Pulse
  */
  virtual bool ArmEvent(struct sigevent* event) = 0;
/**
  * @brief This method is invoked from threadMain() when Pulse comes
  */
  virtual void OnPulse() = 0;

  /**
   * This method is to be initialize child class
   * @return If this method returns false, thread will be stopped
   */
  virtual bool Init() { return true; }

/**
  * Finalizes thread
  * Can free resources
  */
  virtual void Finalize() {}

 private:
  enum {PULSE_CODE = _PULSE_CODE_MINAVAIL + 1};

  volatile bool run_;
  int chid_;
  int coid_;
};

}  // namespace threads

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_PULSE_THREAD_DELEGATE_H_
