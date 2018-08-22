/*
 * \file bluetooth_transport_adapter.cc
 * \brief BluetoothTransportAdapter class source file.
 *
 * Copyright (c) 2017, Ford Motor Company
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
#include <sys/socket.h>
#include <unistd.h>

#include <iomanip>
#include <set>
#include <bluetooth/bluetooth.h>

#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"
#include "transport_manager/bluetooth/bluetooth_device_scanner.h"
#include "transport_manager/bluetooth/bluetooth_connection_factory.h"
#include "transport_manager/bluetooth/bluetooth_device.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

BluetoothTransportAdapter::~BluetoothTransportAdapter() {}

BluetoothTransportAdapter::BluetoothTransportAdapter(
    resumption::LastState& last_state, const TransportManagerSettings& settings)
    : TransportAdapterImpl(new BluetoothDeviceScanner(this, true, 0),
                           new BluetoothConnectionFactory(this),
                           NULL,
                           last_state,
                           settings) {}

DeviceType BluetoothTransportAdapter::GetDeviceType() const {
  return BLUETOOTH;
}

void BluetoothTransportAdapter::Store() const {
  LOG4CXX_TRACE(logger_, "enter");
  Json::Value bluetooth_adapter_dictionary;
  Json::Value devices_dictionary;
  DeviceList device_ids = GetDeviceList();
  for (DeviceList::const_iterator i = device_ids.begin(); i != device_ids.end();
       ++i) {
    DeviceUID device_id = *i;
    DeviceSptr device = FindDevice(device_id);
    if (!device) {  // device could have been disconnected
      continue;
    }
    std::shared_ptr<BluetoothDevice> bluetooth_device =
        std::static_pointer_cast<BluetoothDevice>(device);
    Json::Value device_dictionary;
    device_dictionary["name"] = bluetooth_device->name();
    char address[18];
    ba2str(&bluetooth_device->address(), address);
    device_dictionary["address"] = std::string(address);
    Json::Value applications_dictionary;
    ApplicationList app_ids = bluetooth_device->GetApplicationList();
    for (ApplicationList::const_iterator j = app_ids.begin();
         j != app_ids.end();
         ++j) {
      ApplicationHandle app_handle = *j;
      if (FindEstablishedConnection(bluetooth_device->unique_device_id(),
                                    app_handle)) {
        uint8_t rfcomm_channel;
        bluetooth_device->GetRfcommChannel(app_handle, &rfcomm_channel);
        Json::Value application_dictionary;
        char rfcomm_channel_record[4];
        sprintf(rfcomm_channel_record, "%u", rfcomm_channel);
        application_dictionary["rfcomm_channel"] =
            std::string(rfcomm_channel_record);
        applications_dictionary.append(application_dictionary);
      }
    }
    if (!applications_dictionary.empty()) {
      device_dictionary["applications"] = applications_dictionary;
      devices_dictionary.append(device_dictionary);
    }
  }
  bluetooth_adapter_dictionary["devices"] = devices_dictionary;
  Json::Value& dictionary = last_state().get_dictionary();
  dictionary["TransportManager"]["BluetoothAdapter"] =
      bluetooth_adapter_dictionary;
  LOG4CXX_TRACE(logger_, "exit");
}

bool BluetoothTransportAdapter::Restore() {
  LOG4CXX_TRACE(logger_, "enter");
  bool errors_occured = false;
  const Json::Value bluetooth_adapter_dictionary =
      last_state().get_dictionary()["TransportManager"]["BluetoothAdapter"];
  const Json::Value devices_dictionary =
      bluetooth_adapter_dictionary["devices"];
  for (Json::Value::const_iterator i = devices_dictionary.begin();
       i != devices_dictionary.end();
       ++i) {
    const Json::Value device_dictionary = *i;
    std::string name = device_dictionary["name"].asString();
    std::string address_record = device_dictionary["address"].asString();
    bdaddr_t address;
    str2ba(address_record.c_str(), &address);
    RfcommChannelVector rfcomm_channels;
    const Json::Value applications_dictionary =
        device_dictionary["applications"];
    for (Json::Value::const_iterator j = applications_dictionary.begin();
         j != applications_dictionary.end();
         ++j) {
      const Json::Value application_dictionary = *j;
      std::string rfcomm_channel_record =
          application_dictionary["rfcomm_channel"].asString();
      uint8_t rfcomm_channel =
          static_cast<uint8_t>(atoi(rfcomm_channel_record.c_str()));
      rfcomm_channels.push_back(rfcomm_channel);
    }
    BluetoothDevice* bluetooth_device =
        new BluetoothDevice(address, name.c_str(), rfcomm_channels);
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
    LOG4CXX_TRACE(logger_, "exit with TRUE");
  } else {
    LOG4CXX_TRACE(logger_, "exit with FALSE");
  }
  return result;
}

}  // namespace transport_adapter
}  // namespace transport_manager
