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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_CLIENT_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_CLIENT_LISTENER_H_

#include <string>

#include "utils/threads/pulse_thread_delegate.h"
#include "transport_manager/transport_adapter/client_connection_listener.h"
#include "transport_manager/aoa/aoa_wrapper.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Listener of device adapter that use TCP transport.
 */
class AOAClientListener : public ClientConnectionListener {
 public:
  explicit AOAClientListener(TransportAdapterController* controller);

 protected:
  TransportAdapter::Error Init();
  void Terminate();
  bool IsInitialised() const;
  TransportAdapter::Error StartListening();
  TransportAdapter::Error StopListening();

 private:
  static const std::string kPpsPathRoot;
  static const std::string kPpsPathAll;
  static const std::string kPpsPathCtrl;
  TransportAdapterController* controller_;
  int fd_;

  bool OpenPps();
  void ClosePps();
  bool ArmEvent(struct sigevent* event);
  void ProcessPpsData(uint8_t* buf, size_t size);
  void WritePpsData(const char* objname, const char** attrs);
  void FillUsbInfo(const char* objname, const char** attrs,
                   AOAWrapper::AOAUsbInfo* info);

  class PpsThreadDelegate : public threads::PulseThreadDelegate {
   public:
    explicit PpsThreadDelegate(AOAClientListener* parent);

   protected:
    virtual bool Init();
    virtual bool ArmEvent(struct sigevent* event);
    virtual void OnPulse();
    virtual void Finalize();

   private:
    AOAClientListener* parent_;
  };
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_CLIENT_LISTENER_H_
