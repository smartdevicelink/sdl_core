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

void BluetoothAdapter::connectionThread()
{
    LOG4CXX_INFO(logger_, "Connection thread started for connection " << ConnectionHandle);

    tDeviceHandle deviceHandle = InvalidDeviceHandle;
    struct sockaddr_rc remoteSocketAddress = {0};
    remoteSocketAddress.rc_family = AF_BLUETOOTH;

    pthread_mutex_lock(&mConnectionsMutex);

    SRFCOMMConnection * connection = 0;
    tConnectionMap::const_iterator connectionIterator = mConnections.find(ConnectionHandle);

    if (connectionIterator != mConnections.end())
    {
        connection = dynamic_cast<SRFCOMMConnection*>(connectionIterator->second);

        if (0 != connection)
        {
            deviceHandle = connection->mDeviceHandle;
            remoteSocketAddress.rc_channel = connection->mRFCOMMChannel;
        }
        else
        {
            LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " is not valid");
        }
    }
    else
    {
        LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " does not exist");
    }

    pthread_mutex_unlock(&mConnectionsMutex);

    if (0 != connection)
    {
        if (InvalidDeviceHandle != deviceHandle)
        {
            bool isDeviceValid = false;

            pthread_mutex_lock(&mDevicesMutex);

            tDeviceMap::const_iterator deviceIterator = mDevices.find(deviceHandle);

            if (deviceIterator != mDevices.end())
            {
                const SBluetoothDevice * device = dynamic_cast<const SBluetoothDevice*>(deviceIterator->second);

                if (0 != device)
                {
                    isDeviceValid = true;
                    memcpy(&remoteSocketAddress.rc_bdaddr, &device->mAddress, sizeof(bdaddr_t));
                }
                else
                {
                    LOG4CXX_ERROR(logger_, "Device " << deviceHandle << " is not valid");
                }
            }
            else
            {
                LOG4CXX_ERROR(logger_, "Device " << deviceHandle << " does not exist");
            }

            pthread_mutex_unlock(&mDevicesMutex);

            if (true == isDeviceValid)
            {
                int rfcommSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

                if (-1 != rfcommSocket)
                {
                    if (0 == connect(rfcommSocket, (struct sockaddr *)&remoteSocketAddress, sizeof(remoteSocketAddress)))
                    {
                        connection->mConnectionSocket = rfcommSocket;
                        handleCommunication(ConnectionHandle);
                    }
                    else
                    {
                        LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to connect to remote device " << getUniqueDeviceId(remoteSocketAddress.rc_bdaddr) << " for connection " << ConnectionHandle);
                    }
                }
                else
                {
                    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create RFCOMM socket for connection " << ConnectionHandle);
                }
            }
        }
        else
        {
            LOG4CXX_ERROR(logger_, "Device handle for connection " << ConnectionHandle << " is invalid");
        }
    }
    else
    {
        LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " is null");
    }

    LOG4CXX_INFO(logger_, "Removing connection " << ConnectionHandle << " from connection map");

    pthread_mutex_lock(&mConnectionsMutex);
    mConnections.erase(ConnectionHandle);
    pthread_mutex_unlock(&mConnectionsMutex);

    delete connection;

    LOG4CXX_INFO(logger_, "Connection thread finished for connection " << ConnectionHandle);
}


} // namespace transport_manager


