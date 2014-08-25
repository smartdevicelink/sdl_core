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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_WRAPPER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_WRAPPER_H_

#include <stdint.h>
#include <vector>

#include "protocol/common.h"

struct aoa_hdl_s;
struct usb_info_t;

namespace transport_manager {
namespace transport_adapter {

enum AOAVersion {
  AOA_Ver_Unknown,
  AOA_Ver_1_0,
  AOA_Ver_2_0
};
enum AOAMode {
  AOA_Mode_Accessory,
  AOA_Mode_Audio,
  AOA_Mode_Debug
};
enum AOAEndpoint {
  AOA_Ept_Accessory_BulkIn,
  AOA_Ept_Accessory_BulkOut,
  AOA_Ept_Accessory_Control
};

class AOAScannerObserver;
class AOADeviceObserver;

class AOAWrapper {
 public:
  typedef aoa_hdl_s* AOAHandle;
  struct AOAUsbInfo {
    std::string path;  /* Path to the USB stack */
    uint32_t devno;  /* Device number */
    uint32_t busno;  /* Device bus number */
    uint32_t iface;  /* Device interface */
  };

  static bool Init(const std::string& path_to_config,
                   const AOAWrapper::AOAUsbInfo& info,
                   AOAScannerObserver* observer);
  static bool Shutdown();
  static inline bool IsError(int ret);
  static inline void PrintError(int ret);

  explicit AOAWrapper(AOAHandle hdl);
  AOAWrapper(AOAHandle hdl, uint32_t timeout);

  bool IsValidHandle() const;
  AOAVersion GetProtocolVesrion() const;
  uint32_t GetBufferMaximumSize(AOAEndpoint endpoint) const;
  std::vector<AOAMode> GetModes() const;
  std::vector<AOAEndpoint> GetEndpoints() const;
  bool Subscribe(AOADeviceObserver* observer) const;
  bool Unsubscribe() const;
  bool SendMessage(RawMessagePtr message) const;
  bool SendControlMessage(uint16_t request, uint16_t value, uint16_t index,
                          RawMessagePtr message) const;
  bool ReceiveMessage(RawMessagePtr *message) const;
  bool ReceiveControlMessage(uint16_t request, uint16_t value, uint16_t index,
                             RawMessagePtr *message) const;

 private:
  AOAHandle hdl_;
  uint32_t timeout_;

  inline AOAVersion Version(uint16_t version) const;
  inline uint32_t BitEndpoint(AOAEndpoint endpoint) const;
  inline bool IsValueExistInMask(uint32_t bitmask, uint32_t value) const;
  std::vector<AOAMode> CreateModesList(uint32_t modes_mask) const;
  std::vector<AOAEndpoint> CreateEndpointsList(uint32_t endpoints_mask) const;
  bool SetCallback(AOADeviceObserver *observer, AOAEndpoint endpoint) const;
  bool UnsetCallback(AOAEndpoint endpoint) const;
  void PrepareUsbInfo(const AOAUsbInfo& aoa_usb_info, usb_info_t* usb_info);
};

class AOAScannerObserver {
 public:
  virtual void OnConnectedDevice(AOAWrapper::AOAHandle handle) = 0;
  virtual ~AOAScannerObserver() {
  }
};

class AOADeviceObserver {
 public:
  virtual void OnReceivedMessage(bool success, RawMessagePtr message) = 0;
  virtual void OnTransmittedMessage(bool success, RawMessagePtr message) = 0;
  virtual ~AOADeviceObserver() {
  }
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_WRAPPER_H_
