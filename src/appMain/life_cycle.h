/*
* Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_APPMAIN_LIFE_CYCLE_H_
#define SRC_APPMAIN_LIFE_CYCLE_H_

namespace main_namespace {

/**
 * Class responsible for all system components creation,
 * start, stop, suspend and restore
 */

class LifeCycle {
 public:
  virtual ~LifeCycle() {}

  /**
   * Creates and starts all system components
   * @return true if all components started successfully
   * otherwise false.
   */
  virtual bool StartComponents() = 0;

  /**
   * Initializes MessageBroker component
   * @return true if success otherwise false.
   */
  virtual bool InitMessageSystem() = 0;

  /**
   * @brief Main loop
   */
  virtual void Run() = 0;

  /**
   * Stops all system components
   */
  virtual void StopComponents() = 0;

  /**
   * Makes appropriate actions when Low Voltage signal received:
   * Stops all SDL activities except of waiting of UNIX signals
   * from HMI
   */
  virtual void LowVoltage() = 0;

  /**
   * Makes appropriate actions when Wake Up signal received:
   * Restores all SDL activities stopped due to LOW VOLTAGE
   * from HMI
   */
  virtual void WakeUp() = 0;

  /**
   * Makes appropriate actions when Ignition Off signal received:
   * Triggers all SDL components stop and deletion
   */
  virtual void IgnitionOff() = 0;
};
}  //  namespace main_namespace

#endif  // SRC_APPMAIN_LIFE_CYCLE_H_
