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
  LOG4CXX_INFO(logger_, "BluetoothAdapter constructed");

  uint8_t smart_device_link_service_uuid_data[] = { 0x93, 0x6D, 0xA0, 0x1F,
      0x9A, 0xBD, 0x4D, 0x9D, 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8 };
  sdp_uuid128_create(&smart_device_link_service_uuid_,
                     smart_device_link_service_uuid_data);
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
    const DeviceHandle device_handle, const RawMessageSptr data_container) {
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

void BluetoothAdapter::mainThread() {
  LOG4CXX_INFO(logger_, "Bluetooth adapter main thread initialized");

  const size_t max_devices = 256u;
  inquiry_info* inquiry_info_list = new inquiry_info[max_devices];

  while (false == shutdown_flag_) {
    DeviceMap new_devices;
    DeviceVector discovered_devices;

    bool device_scan_requested = waitForDeviceScanRequest(0);

    if (device_scan_requested) {
      bool device_scan_succeeded = false;
      int device_id = hci_get_route(0);

      if (device_id >= 0) {
        int device_handle = hci_open_dev(device_id);

        if (device_handle >= 0) {
          const uint8_t inquiry_time = 8u;  // Time unit is 1.28 seconds

          LOG4CXX_INFO(logger_, "Starting hci_inquiry on device " << device_id);

          int number_of_devices = hci_inquiry(device_id, inquiry_time,
                                              max_devices, 0,
                                              &inquiry_info_list,
                                              IREQ_CACHE_FLUSH);

          if (number_of_devices >= 0) {
            LOG4CXX_INFO(
                logger_,
                "hci_inquiry: found " << number_of_devices << " devices");

            device_scan_succeeded = true;

            for (int i = 0; i < number_of_devices; ++i) {
              RfcommChannelVector smart_device_link_rfcomm_channels;
              discoverSmartDeviceLinkRfcommChannels(
                  inquiry_info_list[i].bdaddr,
                  &smart_device_link_rfcomm_channels);

              if (false == smart_device_link_rfcomm_channels.empty()) {
                char deviceName[256];

                if (0
                    != hci_read_remote_name(
                        device_handle, &inquiry_info_list[i].bdaddr,
                        sizeof(deviceName) / sizeof(deviceName[0]), deviceName,
                        0)) {
                  LOG4CXX_ERROR_WITH_ERRNO(logger_,
                                           "hci_read_remote_name failed");
                  strncpy(
                      deviceName,
                      getUniqueDeviceId(inquiry_info_list[i].bdaddr).c_str(),
                      sizeof(deviceName) / sizeof(deviceName[0]));
                }

                Device* bluetooth_device = new BluetoothDevice(
                    inquiry_info_list[i].bdaddr, deviceName,
                    smart_device_link_rfcomm_channels);
                if (bluetooth_device) {
                  discovered_devices.push_back(bluetooth_device);
                } else {
                  //TODO
                }
              }
            }
          } else {
            LOG4CXX_ERROR(logger_, "hci_inquiry failed");
          }

          close (deviceHandle);
        }
      }

      for (DeviceVector::iterator it = discovered_devices.begin();
          it != discovered_devices.end(); ++it) {
        Device* discovered_device = *it;

        DeviceHandle device_handle;
        bool device_found = false;

        pthread_mutex_lock(&devices_mutex_);

        for (DeviceMap::iterator it = devices_.begin(); it != devices_.end();
            ++it) {
          Device* existing_device = it->second;

          if (true == discovered_device->isSameAs(existing_device)) {
            device_handle = it->first;
            device_found = true;
            break;
          }
        }

        pthread_mutex_unlock(&devices_mutex_);

        if (!device_found) {
          device_handle = handle_generator_->generate();

          LOG4CXX_INFO(
              logger_,
              "Adding new device " << device_handle << " (\"" << discovered_device->name_ << "\")");
        }

        new_devices[device_handle] = discovered_devices;
      }

      pthread_mutex_lock(&connections_mutex_);

      std::set<DeviceHandle> connected_devices;

      for (ConnectionMap::const_iterator it = connections_.begin();
          it != connections_.end(); ++it) {
        const Connection* connection = it->second;

        if (connected_devices.end()
            == connected_devices.find(connection->device_handle())) {
          connected_devices.insert(connection->device_handle());
        }
      }

      pthread_mutex_unlock(&connections_mutex_);

      pthread_mutex_lock(&devices_mutex_);

      for (DeviceMap::iterator it = devices_.begin(); it != devices_.end();
          ++it) {
        Device* device = it->second;

        if (new_devices.end() == new_devices.find(it->first)) {
          if (connected_devices.end() != connected_devices.find(it->first)) {
            new_devices[it->first] = device;
            device = 0;
          }
        }

        delete device;
      }

      devices_ = new_devices;

      pthread_mutex_unlock(&devices_mutex_);

      LOG4CXX_INFO(
          logger_,
          "Discovered " << new_devices.size() << " device" << ((1u == new_devices.size()) ? "" : "s") << " with SmartDeviceLink service. New devices map:");

      for (DeviceMap::iterator it = new_devices.begin();
          it != new_devices.end(); ++it) {
        Device* device = it->second;

        if (0 != device) {
          LOG4CXX_INFO(
              logger_,
              std::setw(10) << it->first << std::setw(0) << ": " << device->unique_device_id_ << ", " << device->name_);
        } else {
          LOG4CXX_ERROR(
              logger_,
              std::setw(10) << it->first << std::setw(0) << ": Device is null");
        }
      }

      device_scan_requested_ = false;

      if (device_scan_succeeded) {
        listener_.onSearchDeviceDone(this);
      } else {
        listener_.onSearchDeviceFailed(this, SearchDeviceError());
      }
    }
  }

  delete[] inquiry_info_list;

  LOG4CXX_INFO(logger_, "Bluetooth adapter main thread finished");
}

ApplicationList BluetoothAdapter::getApplicationList(
    const DeviceHandle device_handle) const {
  ApplicationList result;

  pthread_mutex_lock(&devices_mutex_);

  DeviceMap::const_iterator it = devices_.find(device_handle);
  if (it != devices_.end()) {
    const Device* device = it->second;

    const BluetoothDevice* bluetooth_device =
        dynamic_cast<const BluetoothDevice*>(device);
    if (bluetooth_device) {
      const RfcommChannels& channels = bluetooth_device->rfcomm_channels();
      for (RfcommChannels::const_iterator it = channels.begin();
          it != channels.end(); ++it) {
        result.push_back(it->first);
      }
    }
  }

  pthread_mutex_unlock(&devices_mutex_);

  return result;
}

void BluetoothAdapter::discoverSmartDeviceLinkRfcommChannels(
    const bdaddr_t& device_address, RfcommChannelVector* channels) {
  channels->clear();

  static bdaddr_t anyAddress = { { 0, 0, 0, 0, 0, 0 } };

  sdp_session_t* sdp_session = sdp_connect(&anyAddress, &device_address,
                                           SDP_RETRY_IF_BUSY);

  if (0 != sdp_session) {
    sdp_list_t* search_list = sdp_list_append(0,
                                              &smart_device_link_service_uuid_);
    uint32_t range = 0x0000ffff;
    sdp_list_t* attr_list = sdp_list_append(0, &range);
    sdp_list_t* response_list = 0;

    if (0
        == sdp_service_search_attr_req(sdp_session, search_list,
                                       SDP_ATTR_REQ_RANGE, attr_list,
                                       &response_list)) {
      for (sdp_list_t* r = response_list; 0 != r; r = r->next) {
        sdp_record_t* sdp_record = static_cast<sdp_record_t*>(r->data);
        sdp_list_t* proto_list = 0;

        if (0 == sdp_get_access_protos(sdp_record, &proto_list)) {
          for (sdp_list_t* p = proto_list; 0 != p; p = p->next) {
            sdp_list_t* pdsList = static_cast<sdp_list_t*>(p->data);

            for (sdp_list_t* pds = pdsList; 0 != pds; pds = pds->next) {
              sdp_data_t* sdpData = static_cast<sdp_data_t*>(pds->data);
              int proto = 0;

              for (sdp_data_t * d = sdpData; 0 != d; d = d->next) {
                switch (d->dtd) {
                  case SDP_UUID16:
                  case SDP_UUID32:
                  case SDP_UUID128:
                    proto = sdp_uuid_to_proto(&d->val.uuid);
                    break;

                  case SDP_UINT8:
                    if (RFCOMM_UUID == proto) {
                      channels->push_back(d->val.uint8);
                    }
                    break;
                }
              }
            }

            sdp_list_free(pdsList, 0);
          }

          sdp_list_free(proto_list, 0);
        }
      }
    }

    sdp_list_free(search_list, 0);
    sdp_list_free(attr_list, 0);
    sdp_list_free(response_list, 0);
    sdp_close(sdp_session);
  } else {
    LOG4CXX_ERROR(
        logger_,
        "Service discovery failed for " << getUniqueDeviceId(device_address));
  }

  if (!channels->empty()) {
    std::stringstream rfcomm_channels_string;

    for (RfcommChannelVector::const_iterator it = channels->begin();
        it != channels->end(); ++it) {
      if (it != channels->begin()) {
        rfcomm_channels_string << ", ";
      }

      rfcomm_channels_string << static_cast<uint32_t>(*it);
    }

    LOG4CXX_INFO(
        logger_,
        "SmartDeviceLink service was discovered on device " << getUniqueDeviceId(device_address) << " at channel(s): " << rfcomm_channels_string.str().c_str());
  } else {
    LOG4CXX_INFO(
        logger_,
        "SmartDeviceLink service was not discovered on device " << getUniqueDeviceId(device_address));
  }
}

std::string BluetoothAdapter::getUniqueDeviceId(
    const bdaddr_t& device_address) {
  char device_address_string[32];

  ba2str(&device_address, device_address_string);

  return std::string("BT-") + device_address_string;
}

BluetoothAdapter::BluetoothDevice::BluetoothDevice(
    const bdaddr_t& address, const char* name,
    const RfcommChannelVector& rfcomm_channels)
    : Device(name),
      address_(address),
      next_application_handle_(1) {

  unique_device_id_ = getUniqueDeviceId(address);
  for (RfcommChannelVector::const_iterator it = rfcomm_channels.begin();
      it != rfcomm_channels.end(); ++it) {
    rfcomm_channels_[next_application_handle_++] = *it;
  }
}

bool BluetoothAdapter::BluetoothDevice::isSameAs(const Device* other) const {
  bool result = false;

  if (Device::isSameAs(other)) {
    const BluetoothDevice* other_bluetooth_device =
        dynamic_cast<const BluetoothDevice*>(other);

    if (0 != other_bluetooth_device) {
      result = (0
          == memcmp(&address_, &other_bluetooth_device->address_,
                    sizeof(bdaddr_t)));
    }
  }

  return result;
}

}  // namespace transport_manager

