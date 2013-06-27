/**
 * \file bluetooth_adapter.h
 * \brief BluetoothAdapter class header file.
 *
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_ADAPTER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_ADAPTER

#include "bluetooth_adapter.h"
#include "device_adapter_impl.h"

namespace transport_manager {

class BluetoothAdapter : public DeviceAdapterImpl {
 public:
  BluetoothAdapter();
  virtual ~BluetoothAdapter();

 protected:

  virtual DeviceType GetDeviceType() const;

  virtual Error init(DeviceAdapterListener* listener,
                     DeviceHandleGenerator* handle_generator,
                     Configuration* configuration);

  virtual bool isSearchDevicesSupported() const;
  virtual Error searchDevices();

  virtual bool isServerOriginatedConnectSupported() const;
  virtual Error connectDevice(const DeviceHandle device_handle);

  virtual bool isClientOriginatedConnectSupported() const;

  virtual DeviceList getDeviceList() const;

  virtual void connectionThread(Connection* connection);

  virtual ApplicationList getApplicationList(
      const DeviceHandle device_handle) const;

  /**
   * @brief Get unique device ID.
   *
   * Get TransportManager-unique identifier of
   * bluetooth device based on its bluetooth address.
   *
   * @param DeviceAddress Address of device.
   *
   * @return Unique device identifier.
   **/
  static std::string getUniqueDeviceId(const bdaddr_t& DeviceAddress);

 protected:
  typedef std::vector<uint8_t> RfcommChannelVector;
  typedef std::map<ApplicationHandle, uint8_t> RfcommChannels;

  class BluetoothDevice : public Device {
   public:
    /**
     * @brief Constructor.
     *
     * @param address Bluetooth address.
     * @param name Human-readable device name.
     * @param rfcomm_channels List of RFCOMM channels where SmartDeviceLink service has been discovered.
     **/
    BluetoothDevice(const bdaddr_t& address, const char* name,
                    const RfcommChannelVector& rfcomm_channels);

    /**
     * @brief Compare devices.
     *
     * This method checks whether two SBluetoothDevice structures
     * refer to the same device.
     *
     * @param other Device to compare with.
     *
     * @return true if devices are equal, false otherwise.
     **/
    virtual bool isSameAs(const Device* other) const;

    const RfcommChannels& rfcomm_channels() const {
      return rfcomm_channels_;
    }

    const bdaddr_t& address() const {
      return address_;
    }

   private:
    /**
     * @brief Device bluetooth address.
     **/
    bdaddr_t address_;

    /**
     * @brief List of RFCOMM channels where SmartDeviceLink service has been discovered.
     **/
    RfcommChannels rfcomm_channels_;

    ApplicationHandle next_application_handle_;
  };

 protected:
  void discoverSmartDeviceLinkRfcommChannels(const bdaddr_t& device_address,
                                             RfcommChannelVector* channels);

 private:
  bool initialized_;
  DeviceHandleGenerator* handle_generator_;

  uuid_t smart_device_link_service_uuid_;
};

}  // namespace transport_manager

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_ADAPTER
