/**
 * \file BluetoothAdapter.cpp
 * \brief BluetoothAdapter class source file.
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

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "transport_manager/bluetooth_adapter.h"

namespace transport_manager {

BluetoothAdapter::BluetoothAdapter()
    : initialized_(false),
      listener_(0),
      handle_generator_(0) {
}

BluetoothAdapter::~BluetoothAdapter() {
}

DeviceType transport_manager::BluetoothAdapter::GetDeviceType() const {
  return "bluetooth-default";
}

DeviceAdapter::Error BluetoothAdapter::init(
    DeviceAdapterListener* listener, DeviceHandleGenerator* handle_generator,
    Configuration* configuration) {
  assert(listener != 0);
  assert(handle_generator != 0);

  listener_ = listener;
  handle_generator_ = handle_generator;
  initialized_ = true;

  return DeviceAdapter::OK;
}

bool BluetoothAdapter::isSearchDevicesSupported() const {
  return true;
}

DeviceAdapter::Error BluetoothAdapter::searchDevices() {
  if (!initialized_) {
    return DeviceAdapter::BAD_STATE;
  }

  //TODO

  return DeviceAdapter::OK;
}

bool BluetoothAdapter::isServerOriginatedConnectSupported() const {
  return true;
}

DeviceAdapter::Error BluetoothAdapter::connectDevice(
    const DeviceHandle device_handle) {
  if (!initialized_) {
    return DeviceAdapter::BAD_STATE;
  }

  //TODO

  return DeviceAdapter::OK;
}

bool BluetoothAdapter::isClientOriginatedConnectSupported() const {
  if (!initialized_) {
    return DeviceAdapter::BAD_STATE;
  }

  //TODO

  return DeviceAdapter::OK;
}

DeviceAdapter::Error BluetoothAdapter::disconnectDevice(
    const DeviceHandle device_handle) {
  if (!initialized_) {
    return DeviceAdapter::BAD_STATE;
  }

  //TODO

  return DeviceAdapter::OK;
}

DeviceAdapter::Error BluetoothAdapter::sendData(
    const DeviceHandle device_handle, const DataContainerSptr data_container) {
  if (!initialized_) {
    return DeviceAdapter::BAD_STATE;
  }

  //TODO

  return DeviceAdapter::OK;
}

DeviceList BluetoothAdapter::getDeviceList() const {
  return DeviceList();
}

void BluetoothAdapter::connectionThread(Connection* connection) {
  assert(connection != 0);
  LOG4CXX_INFO(
      logger_,
      "Connection thread started for session " << connection->session_id());

  const DeviceHandle device_handle = connection->device_handle();
  const ApplicationHandle app_handle = connection->application_handle();
  const int session_id = connection->session_id();

  pthread_mutex_lock(&devices_mutex_);
  BluetoothDevice* device = 0;
  DeviceMap::const_iterator device_it = devices_.find(
      connection->device_handle());
  if (device_it == devices_.end()) {
    LOG4CXX_ERROR(logger_, "Device " << device_handle << " does not exist");
  } else {
    device = dynamic_cast<BluetoothDevice*>(device_it->second);
    if (device == 0) {
      LOG4CXX_ERROR(logger_, "Device " << device_handle << " is not valid");
    }
  }
  pthread_mutex_unlock(&devices_mutex_);

  if (device != 0) {
    RfcommChannels::const_iterator rfcomm_channels_it =
        device->rfcomm_channels().find(app_handle);
    if (device->rfcomm_channels().end() == rfcomm_channels_it) {
      LOG4CXX_ERROR(
          logger_,
          "Device " << device_handle << " with application " << app_handle << " not found");
    } else {
      struct sockaddr_rc remoteSocketAddress = { 0 };
      remoteSocketAddress.rc_family = AF_BLUETOOTH;
      memcpy(&remoteSocketAddress.rc_bdaddr, &device->address(),
             sizeof(bdaddr_t));
      remoteSocketAddress.rc_channel = rfcomm_channels_it->second;

      const int rfcommSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

      if (-1 != rfcommSocket) {
        if (0
            == ::connect(rfcommSocket, (struct sockaddr *) &remoteSocketAddress,
                         sizeof(remoteSocketAddress))) {
          connection->set_connection_socket(rfcommSocket);
          handleCommunication(connection);
        } else {
          LOG4CXX_ERROR_WITH_ERRNO(
              logger_,
              "Failed to connect to remote device " << getUniqueDeviceId(remoteSocketAddress.rc_bdaddr) << " for session " << session_id);
        }
      } else {
        LOG4CXX_ERROR_WITH_ERRNO(
            logger_,
            "Failed to create RFCOMM socket for device " << device_handle);
      }
    }
  }

  LOG4CXX_INFO(
      logger_,
      "Removing connection for session " << session_id << " from connection map");

  pthread_mutex_lock(&connections_mutex_);
  connections_.erase(connection->session_id());
  pthread_mutex_unlock(&connections_mutex_);

  delete connection;

  LOG4CXX_INFO(logger_, "Connection thread finished for session " << session_id);
}


} // namespace transport_manager


