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

#include <sys/types.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>

#include <iomanip>
#include <set>

#include "transport_manager/bluetooth_adapter.h"
#include "transport_manager/device_adapter_listener.h"
#include "transport_manager/device_adapter_impl.h"

namespace transport_manager {

namespace device_adapter {

bool BluetoothDevice::getRfcommChannel(const ApplicationHandle app_handle,
                                       uint8_t* channel_out) {
  RfcommChannels::const_iterator it = rfcomm_channels_.find(app_handle);
  if (it != rfcomm_channels_.end()) {
    *channel_out = it->second;
    return true;
  } else {
    return false;
  }
}

std::string getUniqueDeviceId(const bdaddr_t& device_address) {
  char device_address_string[32];
  ba2str(&device_address, device_address_string);
  return std::string("BT-") + device_address_string;
}

BluetoothDeviceScanner::BluetoothDeviceScanner(
    DeviceAdapterController* controller)
    : controller_(controller),
      thread_(),
      thread_started_(false),
      shutdown_requested_(false),
      device_scan_requested_(false),
      device_scan_requested_mutex_(),
      device_scan_requested_cond_() {
  pthread_cond_init(&device_scan_requested_cond_, 0);
  pthread_mutex_init(&device_scan_requested_mutex_, 0);

  uint8_t smart_device_link_service_uuid_data[] = { 0x93, 0x6D, 0xA0, 0x1F,
      0x9A, 0xBD, 0x4D, 0x9D, 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8 };
  sdp_uuid128_create(&smart_device_link_service_uuid_,
                     smart_device_link_service_uuid_data);
}

BluetoothDeviceScanner::~BluetoothDeviceScanner() {
  pthread_mutex_destroy(&device_scan_requested_mutex_);
  pthread_cond_destroy(&device_scan_requested_cond_);
}

void* bluetoothDeviceScannerThread(void* data) {
  BluetoothDeviceScanner* bluetoothDeviceScanner =
      static_cast<BluetoothDeviceScanner*>(data);
  assert(bluetoothDeviceScanner != 0);
  bluetoothDeviceScanner->thread();
  return 0;
}

SearchDeviceError* BluetoothDeviceScanner::doInquiry(
    DeviceVector* discovered_devices) {
  assert(discovered_devices != 0);

  const int device_id = hci_get_route(0);

  if (device_id < 0) {
    return new SearchDeviceError();  //FIXME
  }

  int device_handle = hci_open_dev(device_id);

  if (device_handle < 0) {
    return new SearchDeviceError();  //FIXME
  }

  LOG4CXX_INFO(logger_, "Starting hci_inquiry on device " << device_id);

  const uint8_t inquiry_time = 8u;  // Time unit is 1.28 seconds
  const size_t max_devices = 256u;
  inquiry_info* inquiry_info_list = new inquiry_info[max_devices];

  const int number_of_devices = hci_inquiry(device_id, inquiry_time,
                                            max_devices, 0, &inquiry_info_list,
                                            IREQ_CACHE_FLUSH);

  if (number_of_devices >= 0) {
    LOG4CXX_INFO(logger_,
                 "hci_inquiry: found " << number_of_devices << " devices");

    for (int i = 0; i < number_of_devices; ++i) {
      RfcommChannelVector smart_device_link_rfcomm_channels =
          discoverSmartDeviceLinkRfcommChannels(inquiry_info_list[i].bdaddr);

      if (smart_device_link_rfcomm_channels.empty())
        continue;

      char deviceName[256];
      int hci_read_remote_name_ret = hci_read_remote_name(
          device_handle, &inquiry_info_list[i].bdaddr,
          sizeof(deviceName) / sizeof(deviceName[0]), deviceName, 0);

      if (hci_read_remote_name_ret != 0) {
        LOG4CXX_ERROR_WITH_ERRNO(logger_, "hci_read_remote_name failed");
        strncpy(deviceName,
                getUniqueDeviceId(inquiry_info_list[i].bdaddr).c_str(),
                sizeof(deviceName) / sizeof(deviceName[0]));
      }

      Device* bluetooth_device = new BluetoothDevice(
          inquiry_info_list[i].bdaddr, deviceName,
          smart_device_link_rfcomm_channels);
      if (bluetooth_device) {
        discovered_devices->push_back(bluetooth_device);
      } else {
        //TODO
      }
    }
  }

  close(device_handle);
  delete[] inquiry_info_list;

  if (number_of_devices < 0) {
    return new SearchDeviceError();  //FIXME
  }

  return 0;
}

BluetoothDeviceScanner::RfcommChannelVector BluetoothDeviceScanner::discoverSmartDeviceLinkRfcommChannels(
    const bdaddr_t& device_address) {
  RfcommChannelVector channels;

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
                      channels.push_back(d->val.uint8);
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

  if (!channels.empty()) {
    std::stringstream rfcomm_channels_string;

    for (RfcommChannelVector::const_iterator it = channels.begin();
        it != channels.end(); ++it) {
      if (it != channels.begin()) {
        rfcomm_channels_string << ", ";
      }

      rfcomm_channels_string << static_cast<uint32_t>(*it);
    }

    LOG4CXX_INFO(
        logger_,
        "SmartDeviceLink service was discovered on device "
            << getUniqueDeviceId(device_address) << " at channel(s): "
            << rfcomm_channels_string.str().c_str());
  } else {
    LOG4CXX_INFO(
        logger_,
        "SmartDeviceLink service was not discovered on device "
            << getUniqueDeviceId(device_address));
  }

  return channels;
}

void BluetoothDeviceScanner::thread() {
  LOG4CXX_INFO(logger_, "Bluetooth adapter main thread initialized");

  while (false == shutdown_requested_) {
    bool device_scan_requested = waitForDeviceScanRequest();
    if (device_scan_requested) {
      DeviceVector discovered_devices;
      SearchDeviceError* error = doInquiry(&discovered_devices);

      if (error == 0) {
        LOG4CXX_INFO(
            logger_,
            "Discovered " << discovered_devices.size() << " device" << ((1u == discovered_devices.size()) ? "" : "s") << " with SmartDeviceLink service:");
        for (DeviceVector::iterator it = discovered_devices.begin();
            it != discovered_devices.end(); ++it) {
          DeviceSptr device = *it;
          LOG4CXX_INFO(logger_,
                       device->unique_device_id() << ", " << device->name());
        }
        controller_->searchDeviceDone(discovered_devices);
      } else {
        controller_->searchDeviceFailed(*error);
      }
    }
    device_scan_requested_ = false;
  }

  LOG4CXX_INFO(logger_, "Bluetooth device scanner thread finished");
}

bool BluetoothDeviceScanner::waitForDeviceScanRequest() {
  bool deviceScanRequested = false;

  pthread_mutex_lock(&device_scan_requested_mutex_);

  if (false == device_scan_requested_) {
    if (0
        != pthread_cond_wait(&device_scan_requested_cond_,
                             &device_scan_requested_mutex_)) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "pthread_cond_wait failed");
    }
  }

  deviceScanRequested = device_scan_requested_;

  pthread_mutex_unlock(&device_scan_requested_mutex_);

  return deviceScanRequested;
}

DeviceAdapter::Error BluetoothDeviceScanner::init() {
  const int thread_start_error = pthread_create(&thread_, 0,
                                                &bluetoothDeviceScannerThread,
                                                this);

  if (0 == thread_start_error) {
    thread_started_ = true;
    LOG4CXX_INFO(logger_, "Bluetooth device scanner thread started");
  } else {
    LOG4CXX_ERROR(
        logger_,
        "Bluetooth device scanner thread start failed, error code " << thread_start_error);
    return DeviceAdapter::FAIL;
  }

  return DeviceAdapter::OK;
}

void BluetoothDeviceScanner::terminate() {
  shutdown_requested_ = true;

  if (true == thread_started_) {
    pthread_mutex_lock(&device_scan_requested_mutex_);
    device_scan_requested_ = false;
    pthread_cond_signal(&device_scan_requested_cond_);
    pthread_mutex_unlock(&device_scan_requested_mutex_);
    LOG4CXX_INFO(logger_,
                 "Waiting for bluetooth device scanner thread termination");
    pthread_join(thread_, 0);
    LOG4CXX_INFO(logger_, "Bluetooth device scanner thread terminated");
  }
}

DeviceAdapter::Error BluetoothDeviceScanner::scan() {
  if ((!thread_started_) && shutdown_requested_) {
    return DeviceAdapter::BAD_STATE;
  }
  DeviceAdapter::Error ret = DeviceAdapter::OK;

  pthread_mutex_lock(&device_scan_requested_mutex_);

  if (false == device_scan_requested_) {
    LOG4CXX_INFO(logger_, "Requesting device scan");

    device_scan_requested_ = true;
    pthread_cond_signal(&device_scan_requested_cond_);
  } else {
    ret = DeviceAdapter::BAD_STATE;
    LOG4CXX_INFO(logger_, "Device scan is currently in progress");
  }

  pthread_mutex_unlock(&device_scan_requested_mutex_);

  return ret;
}

BluetoothDevice::BluetoothDevice(const bdaddr_t& address, const char* name,
                                 const RfcommChannelVector& rfcomm_channels)
    : Device(name),
      address_(address),
      next_application_handle_(1) {
  set_unique_device_id(getUniqueDeviceId(address));
  for (RfcommChannelVector::const_iterator it = rfcomm_channels.begin();
      it != rfcomm_channels.end(); ++it) {
    rfcomm_channels_[next_application_handle_++] = *it;
  }
}

bool BluetoothDevice::isSameAs(const Device* other) const {
  bool result = false;

  const BluetoothDevice* other_bluetooth_device =
      dynamic_cast<const BluetoothDevice*>(other);

  if (0 != other_bluetooth_device) {
    if (0
        == memcmp(&address_, &other_bluetooth_device->address_,
                  sizeof(bdaddr_t))) {
      result = true;
    }
  }

  return result;
}

BluetoothSocketConnection::BluetoothSocketConnection(
    const DeviceHandle device_handle, const ApplicationHandle app_handle,
    const SessionID session_id, DeviceAdapterController* controller)
    : ThreadedSocketConnection(device_handle, app_handle, session_id,
                               controller) {
}

BluetoothSocketConnection::~BluetoothSocketConnection() {
}

bool BluetoothSocketConnection::establish(ConnectError** error) {
  DeviceSptr device = getController()->findDevice(device_handle());

  BluetoothDevice* bluetooth_device =
      static_cast<BluetoothDevice*>(device.get());

  uint8_t rfcomm_channel;
  if (!bluetooth_device->getRfcommChannel(application_handle(), &rfcomm_channel)) {
    LOG4CXX_ERROR(logger_,
                  "Application " << application_handle() << " not found");
    *error = new ConnectError();
    return false;
  }

  struct sockaddr_rc remoteSocketAddress = { 0 };
  remoteSocketAddress.rc_family = AF_BLUETOOTH;
  memcpy(&remoteSocketAddress.rc_bdaddr, &bluetooth_device->address(),
         sizeof(bdaddr_t));
  remoteSocketAddress.rc_channel = rfcomm_channel;

  const int rfcomm_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if (-1 == rfcomm_socket) {
    LOG4CXX_ERROR_WITH_ERRNO(
        logger_,
        "Failed to create RFCOMM socket for device " << device_handle());
    *error = new ConnectError();
    return false;
  }

  const int connect_status = ::connect(rfcomm_socket,
                                       (struct sockaddr*) &remoteSocketAddress,
                                       sizeof(remoteSocketAddress));

  if (0 != connect_status) {
    LOG4CXX_ERROR_WITH_ERRNO(
        logger_,
        "Failed to connect to remote device " << getUniqueDeviceId(remoteSocketAddress.rc_bdaddr) << " for session " << session_id());
    *error = new ConnectError();
    return false;
  }

  set_socket(rfcomm_socket);
  return true;
}

BluetoothConnectionFactory::BluetoothConnectionFactory(
    DeviceAdapterController* controller)
    : controller_(controller) {
}

DeviceAdapter::Error BluetoothConnectionFactory::init() {
  return DeviceAdapter::OK;
}

DeviceAdapter::Error BluetoothConnectionFactory::createConnection(DeviceHandle device_handle,
                                                   ApplicationHandle app_handle,
                                                   SessionID session_id) {
  BluetoothSocketConnection* connection(
      new BluetoothSocketConnection(device_handle, app_handle, session_id,
                                    controller_));
  DeviceAdapter::Error error = connection->start();
  if (error != DeviceAdapter::OK)
    delete connection;
  return error;
}

BluetoothConnectionFactory::~BluetoothConnectionFactory() {
}

}  // namespace device_adapter

}  // namespace transport_manager

