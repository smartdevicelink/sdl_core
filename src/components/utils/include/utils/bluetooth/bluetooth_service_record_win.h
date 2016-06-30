/*
 * \file bluetooth_service_record_win.h
 * \brief Header for classes which parses and stores bluetooth-service info.
 * Copyright (c) 2016, Ford Motor Company
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
#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_SERVICE_RECORD_WIN_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_SERVICE_RECORD_WIN_H_

#include "utils/winhdr.h"
#include "utils/bluetooth/bluetooth_uuid_win.h"

#include <string>
#include <vector>

namespace utils {
class BluetoothServiceRecordWin {
 public:
  BluetoothServiceRecordWin(const std::string& device_address,
                            const std::string& name,
                            const std::vector<uint8_t>& sdp_bytes,
                            const utils::BluetoothUUID& get_uuid)
      : device_bth_addr_(utils::StringToBthAddr(device_address))
      , device_address_(device_address)
      , name_(name)
      , uuid_(get_uuid)
      , supports_rfcomm_(false)
      , rfcomm_channel_(-1) {
    if (sdp_bytes.size() > 0u) {
      // BluetoothSdpGetAttributeValue requires non const input params,
      // therefore we use const_cast to remove constness from the sdp_bytes
      // to prevent array redundant copy.
      LPBYTE blob_data = const_cast<LPBYTE>(&sdp_bytes[0]);
      ULONG blob_size = static_cast<ULONG>(sdp_bytes.size());
      SDP_ELEMENT_DATA protocol_descriptor_list_data;
      if (ERROR_SUCCESS ==
          BluetoothSdpGetAttributeValue(blob_data,
                                        blob_size,
                                        utils::kProtocolDescriptorListId,
                                        &protocol_descriptor_list_data)) {
        utils::ExtractChannels(
            protocol_descriptor_list_data, supports_rfcomm_, rfcomm_channel_);
      }
      SDP_ELEMENT_DATA uuid_data;
      if (ERROR_SUCCESS ==
          BluetoothSdpGetAttributeValue(
              blob_data, blob_size, utils::kUuidId, &uuid_data)) {
        utils::ExtractUuid(uuid_data, uuid_);
      }
    }
  }

  /**
   * @brief return the BTH_ADDR address of the bluetooth device providing this
   * service.
   */
  BTH_ADDR DeviceBthAddr() const {
    return device_bth_addr_;
  }

  /**
   * @brief The address of the bluetooth device providing this service.
   *
   * @return the address of the bluetooth device converted to std::string.
   */
  const std::string& DeviceAddress() const {
    return device_address_;
  }

  /**
   * @brief Return the human-readable name of this service.
   */
  const std::string& GetServiceName() const {
    return name_;
  }

  /**
   * @brief UUID of the service.  This field may be empty if no UUID was
   *        specified in the service record.
   *
   * @return BluetoothUUID class.
   */
  const utils::BluetoothUUID& Uuid() const {
    return uuid_;
  }

  /**
   * @brief Check if equil stored UUID and service_uuid.
   *
   * @return True if service_uuid and stored UUID equil.
   */
  bool IsUuidEqual(const std::string& service_uuid) const {
    return uuid_.Value() == service_uuid;
  }

  /**
   * @brief Indicates if this service supports RFCOMM communication.
   *
   * @return True if this service supports RFCOMM communication.
   */
  bool SupportsRfcomm() const {
    return supports_rfcomm_;
  }

  /**
   * @brief The RFCOMM channel to use, if this service supports RFCOMM
   *communication.
   *
   * @return value is undefined if SupportsRfcomm() returns false.
   */
  uint8_t RfcommChannel() const {
    return rfcomm_channel_;
  }

 private:
  BTH_ADDR device_bth_addr_;
  std::string device_address_;
  std::string name_;
  utils::BluetoothUUID uuid_;

  bool supports_rfcomm_;
  uint8_t rfcomm_channel_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothServiceRecordWin);
};
}  // namespace utils
#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_SERVICE_RECORD_WIN_H_
