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

DeviceType BluetoothAdapter::GetDeviceType() const {
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

      const int rfcommSocket = socket(AF_BLUETOOTH, SOCK_STREAM,
                                      BTPROTO_RFCOMM);

      if (-1 != rfcommSocket) {
        const int connect_status = ::connect(
            rfcommSocket, static_cast<struct sockaddr*>(&remoteSocketAddress),
            sizeof(remoteSocketAddress));
        if (0 == connect_status) {
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

void BluetoothAdapter::mainThread()
{
    LOG4CXX_INFO(logger_, "Bluetooth adapter main thread initialized");

    const size_t maxDevices = 256u;
    inquiry_info* inquiryInfoList = new inquiry_info[maxDevices];

    while (false == shutdown_flag_)
    {
        DeviceMap newDevices;
        DeviceVector discoveredDevices;

        bool deviceScanRequested = waitForDeviceScanRequest(0);

        if (deviceScanRequested)
        {
            int device_id = hci_get_route(0);

            if (device_id >= 0)
            {
                int deviceHandle = hci_open_dev(device_id);

                if (deviceHandle >= 0)
                {
                    const uint8_t inquiry_time = 8u; // Time unit is 1.28 seconds

                    LOG4CXX_INFO(logger_, "Starting hci_inquiry on device " << device_id);

                    int numberOfDevices = hci_inquiry(device_id, inquiry_time, maxDevices, 0, &inquiryInfoList, IREQ_CACHE_FLUSH);

                    if (numberOfDevices >= 0)
                    {
                        LOG4CXX_INFO(logger_, "hci_inquiry: found " << numberOfDevices << " devices");

                        for (int i = 0; i < numberOfDevices; ++i)
                        {
                            RFCOMMChannelVector SmartDeviceLinkRFCOMMChannels;
                            discoverSmartDeviceLinkRFCOMMChannels(inquiryInfoList[i].bdaddr, SmartDeviceLinkRFCOMMChannels);

                            if (false == SmartDeviceLinkRFCOMMChannels.empty())
                            {
                                char deviceName[256];

                                if (0 != hci_read_remote_name(deviceHandle, &inquiryInfoList[i].bdaddr, sizeof(deviceName) / sizeof(deviceName[0]), deviceName, 0))
                                {
                                    LOG4CXX_ERROR_WITH_ERRNO(logger_, "hci_read_remote_name failed");
                                    strncpy(deviceName, getUniqueDeviceId(inquiryInfoList[i].bdaddr).c_str(), sizeof(deviceName) / sizeof(deviceName[0]));
                                }

                                discoveredDevices.push_back(new SBluetoothDevice(inquiryInfoList[i].bdaddr, deviceName, SmartDeviceLinkRFCOMMChannels));
                            }
                        }
                    }
                    else
                    {
                        LOG4CXX_ERROR(logger_, "hci_inquiry failed");
                    }

                    close(deviceHandle);
                }
            }

            for (tDeviceVector::iterator discoveredDeviceIterator = discoveredDevices.begin(); discoveredDeviceIterator != discoveredDevices.end(); ++discoveredDeviceIterator)
            {
                SDevice * discoveredDevice = *discoveredDeviceIterator;

                if (0 != discoveredDevice)
                {
                    tDeviceHandle deviceHandle = InvalidDeviceHandle;

                    pthread_mutex_lock(&mDevicesMutex);

                    for (tDeviceMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
                    {
                        SDevice * exisingDevice = deviceIterator->second;

                        if (true == discoveredDevice->isSameAs(exisingDevice))
                        {
                            deviceHandle = deviceIterator->first;
                            break;
                        }
                    }

                    pthread_mutex_unlock(&mDevicesMutex);

                    if (InvalidDeviceHandle == deviceHandle)
                    {
                        deviceHandle = mHandleGenerator.generateNewDeviceHandle();

                        LOG4CXX_INFO(logger_, "Adding new device " << deviceHandle << " (\"" << discoveredDevice->mName << "\")");
                    }

                    newDevices[deviceHandle] = discoveredDevice;
                }
            }

            pthread_mutex_lock(&mConnectionsMutex);

            std::set<tDeviceHandle> connectedDevices;

            for (tConnectionMap::const_iterator connectionIterator = mConnections.begin(); connectionIterator != mConnections.end(); ++connectionIterator)
            {
                const SConnection * connection = connectionIterator->second;

                if (0 != connection)
                {
                    if (connectedDevices.end() == connectedDevices.find(connection->mDeviceHandle))
                    {
                        connectedDevices.insert(connection->mDeviceHandle);
                    }
                }
            }

            pthread_mutex_unlock(&mConnectionsMutex);

            pthread_mutex_lock(&mDevicesMutex);

            for (tDeviceMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
            {
                SDevice * device = deviceIterator->second;

                if (0 != device)
                {
                    if (newDevices.end() == newDevices.find(deviceIterator->first))
                    {
                        if (connectedDevices.end() != connectedDevices.find(deviceIterator->first))
                        {
                            newDevices[deviceIterator->first] = device;
                            device = 0;
                        }
                    }

                    if (0 != device)
                    {
                        delete device;
                    }
                }
            }

            mDevices = newDevices;

            pthread_mutex_unlock(&mDevicesMutex);

            LOG4CXX_INFO(logger_, "Discovered " << newDevices.size() << " device" << ((1u == newDevices.size()) ? "" : "s") << " with SmartDeviceLink service. New devices map:");

            for (tDeviceMap::iterator deviceIterator = newDevices.begin(); deviceIterator != newDevices.end(); ++deviceIterator)
            {
                SDevice * device = deviceIterator->second;

                if (0 != device)
                {
                    LOG4CXX_INFO(logger_, std::setw(10) << deviceIterator->first << std::setw(0) << ": " << device->mUniqueDeviceId << ", " << device->mName.c_str());
                }
                else
                {
                    LOG4CXX_ERROR(logger_, std::setw(10) << deviceIterator->first << std::setw(0) << ": Device is null");
                }
            }

            mDeviceScanRequested = false;

            updateClientDeviceList();
        }
    }

    delete [] inquiryInfoList;

    LOG4CXX_INFO(logger_, "Bluetooth adapter main thread finished");
}

} // namespace transport_manager


