/*
 * \file android_transport_adapter.h
 * \brief BluetoothLeAdapter class header file.
 *
 * Copyright (c) 2021, Ford Motor Company
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_ANDROID_TRANSPORT_ADAPTER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_ANDROID_TRANSPORT_ADAPTER_H_

#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

enum class AndroidTransportType { BLE, BT };

/**
 * @brief Transport adapter that use bluetooth low energy transport.
 */
class AndroidTransportAdapter : public TransportAdapterImpl {
 public:
  AndroidTransportAdapter(resumption::LastStateWrapperPtr last_state_wrapper,
                          const TransportManagerSettings& settings,
                          AndroidTransportType transport_type);

  ~AndroidTransportAdapter() override;

  bool ToBeAutoConnected(DeviceSptr device) const override;

  std::string GetSenderSocketName() const;
  std::string GetReceiverSocketName() const;
  std::string GetControlReceiverSocketName() const;

 protected:
  DeviceType GetDeviceType() const override;

  void Store() const override;

  bool Restore() override;

  void SearchDeviceDone(const DeviceVector& devices) override;

  void DisconnectDone(const DeviceUID& device_handle,
                      const ApplicationHandle& app_handle) override;

 private:
  DeviceUID active_device_uid_;
  ApplicationHandle app_handle_;
  AndroidTransportType transport_type_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_ANDROID_ANDROID_TRANSPORT_ADAPTER_H_
