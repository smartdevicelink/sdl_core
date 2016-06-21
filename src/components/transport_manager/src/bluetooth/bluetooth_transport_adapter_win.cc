/*
 * \file bluetooth_transport_adapter.cc
 * \brief BluetoothTransportAdapter class source file.
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

#include <errno.h>
#include <sys/types.h>
#include "utils/winhdr.h"
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#include <sstream>
#include <ws2bth.h>

#include "resumption/last_state.h"
#include <iomanip>
#include <set>

#include "transport_manager/bluetooth/bluetooth_connection_factory.h"
#include "transport_manager/bluetooth/bluetooth_device.h"
#include "transport_manager/bluetooth/bluetooth_device_scanner.h"
#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"

#include "utils/logger.h"
#include "utils/bluetooth/bluetooth_utils_win.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOGGER("TransportManager")

BluetoothTransportAdapter::BluetoothTransportAdapter(
    resumption::LastState& last_state, const TransportManagerSettings& settings)
    : TransportAdapterImpl(
          new BluetoothDeviceScanner(
              this,
              true,
              BluetoothTransportAdapter::kDeviceRepeatSearchIntervalSec),
          new BluetoothConnectionFactory(this),
          0,
          last_state,
          settings) {}

BluetoothTransportAdapter::~BluetoothTransportAdapter() {}

DeviceType BluetoothTransportAdapter::GetDeviceType() const {
  return BLUETOOTH;
}

void BluetoothTransportAdapter::Store() const {
  using namespace utils::json;
  SDL_AUTO_TRACE();
  JsonValue bluetooth_adapter_dictionary;
  JsonValue devices_dictionary;
  DeviceList device_ids = GetDeviceList();
  for (DeviceList::const_iterator i = device_ids.begin(); i != device_ids.end();
       ++i) {
    DeviceUID device_id = *i;
    DeviceSptr device = FindDevice(device_id);
    if (!device) {  // device could have been disconnected
      continue;
    }
    utils::SharedPtr<BluetoothDevice> bluetooth_device =
        DeviceSptr::static_pointer_cast<BluetoothDevice>(device);
    JsonValue device_dictionary;
    device_dictionary["name"] = bluetooth_device->name();
    device_dictionary["address"] =
        utils::BthDeviceAddrToStr(bluetooth_device->address());
    JsonValue applications_dictionary;
    ApplicationList app_ids = bluetooth_device->GetApplicationList();
    devices_dictionary.Append(device_dictionary);
  }
  bluetooth_adapter_dictionary["devices"] = devices_dictionary;

  JsonValue& dictionary = last_state().dictionary();
  dictionary["TransportManager"]["BluetoothAdapter"] =
      bluetooth_adapter_dictionary;
  SDL_TRACE("exit");
}

bool BluetoothTransportAdapter::Restore() {
  using namespace utils::json;
  SDL_AUTO_TRACE();
  bool errors_occured = false;

  const JsonValue& bluetooth_adapter_dictionary =
      last_state().dictionary()["TransportManager"]["BluetoothAdapter"];
  const JsonValueRef devices_dictionary =
      bluetooth_adapter_dictionary["devices"];
  for (JsonValue::const_iterator i = devices_dictionary.begin();
       i != devices_dictionary.end();
       ++i) {
    const JsonValueRef device_dictionary = *i;
    std::string name = device_dictionary["name"].AsString();
    GUID smart_device_link_service_uuid;
    BYTE smart_device_link_service_uuid_data[] = {0x93,
                                                  0x6D,
                                                  0xA0,
                                                  0x1F,
                                                  0x9A,
                                                  0xBD,
                                                  0x4D,
                                                  0x9D,
                                                  0x80,
                                                  0xC7,
                                                  0x02,
                                                  0xAF,
                                                  0x85,
                                                  0xC8,
                                                  0x22,
                                                  0xA8};
    utils::ConvertBytesToUUID(smart_device_link_service_uuid_data,
                              smart_device_link_service_uuid);

    SOCKADDR_BTH sock_addr_bth_server = {0};
    sock_addr_bth_server.addressFamily = AF_BTH;
    sock_addr_bth_server.btAddr =
        utils::StringToSockBthAddr(device_dictionary["address"].AsString())
            .btAddr;
    sock_addr_bth_server.serviceClassId = smart_device_link_service_uuid;

    RfcommChannelVector rfcomm_channels;
    const JsonValue applications_dictionary = device_dictionary["applications"];
    for (JsonValue::const_iterator j = applications_dictionary.begin();
         j != applications_dictionary.end();
         ++j) {
      const JsonValue application_dictionary = *j;
      std::string rfcomm_channel_record =
          application_dictionary["rfcomm_channel"].AsString();
      uint8_t rfcomm_channel =
          static_cast<uint8_t>(atoi(rfcomm_channel_record.c_str()));
      rfcomm_channels.push_back(rfcomm_channel);
    }
    BLUETOOTH_DEVICE_INFO bluetooth_dev_info = {0};
    bluetooth_dev_info.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
    bluetooth_dev_info.Address.ullLong =
        utils::StringToSockBthAddr(device_dictionary["address"].AsString())
            .btAddr;
    bluetooth_dev_info.ulClassofDevice = 0;
    bluetooth_dev_info.fConnected = true;
    bluetooth_dev_info.fRemembered = true;
    bluetooth_dev_info.fAuthenticated = true;

    BluetoothDevice* bluetooth_device =
        new BluetoothDevice(bluetooth_dev_info,
                            name.c_str(),
                            rfcomm_channels,
                            sock_addr_bth_server);
    DeviceSptr device(bluetooth_device);
    AddDevice(device);
    for (RfcommChannelVector::const_iterator j = rfcomm_channels.begin();
         j != rfcomm_channels.end();
         ++j) {
      ApplicationHandle app_handle =
          *j;  // for Bluetooth device app_handle is just RFCOMM channel
      if (Error::OK != Connect(device->unique_device_id(), app_handle)) {
        errors_occured = true;
      }
    }
  }
  bool result = !errors_occured;
  if (result) {
    SDL_TRACE("exit with TRUE");
  } else {
    SDL_TRACE("exit with FALSE");
  }
  return result;
}

}  // namespace transport_adapter
}  // namespace transport_manager
