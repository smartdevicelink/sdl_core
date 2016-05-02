/*
 * \file bluetooth_device_scanner.cc
 * \brief BluetoothDeviceScanner class header file.
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

#include "transport_manager/bluetooth/bluetooth_device_scanner.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"
#include "transport_manager/bluetooth/bluetooth_device.h"

#include "utils/logger.h"
#include "utils/threads/thread.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

namespace {
char* SplitToAddr(char* dev_list_entry) {
  char* bl_address = strtok(dev_list_entry, "()");
  if (bl_address != NULL) {
    bl_address = strtok(NULL, "()");
    return bl_address;
  } else {
    return NULL;
  }
}

int FindPairedDevs(std::vector<bdaddr_t>* result) {
  LOG4CXX_TRACE(logger_, "enter. result adress: " << result);
  DCHECK(result != NULL);

  const char* cmd = "bt-device -l";

  FILE* pipe = popen(cmd, "r");
  if (!pipe) {
    LOG4CXX_TRACE(logger_, "exit -1. Condition: !pipe");
    return -1;
  }
  char* buffer = new char[1028];
  size_t counter = 0;
  while (fgets(buffer, 1028, pipe) != NULL) {
    if (0 < counter++) {  //  skip first line
      char* bt_address = SplitToAddr(buffer);
      if (bt_address) {
        bdaddr_t address;
        str2ba(bt_address, &address);
        result->push_back(address);
      }
    }
    delete [] buffer;
    buffer = new char[1028];
  }
  pclose(pipe);
  LOG4CXX_TRACE(logger_, "exit with 0");
  delete [] buffer;
  return 0;
}
}  //  namespace

BluetoothDeviceScanner::BluetoothDeviceScanner(
  TransportAdapterController* controller, bool auto_repeat_search,
  int auto_repeat_pause_sec)
  : controller_(controller),
    thread_(NULL),
    shutdown_requested_(false),
    ready_(true),
    device_scan_requested_(false),
    device_scan_requested_lock_(),
    device_scan_requested_cv_(),
    auto_repeat_search_(auto_repeat_search),
    auto_repeat_pause_sec_(auto_repeat_pause_sec) {
  uint8_t smart_device_link_service_uuid_data[] = { 0x93, 0x6D, 0xA0, 0x1F,
                                                    0x9A, 0xBD, 0x4D, 0x9D, 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8
                                                  };
  sdp_uuid128_create(&smart_device_link_service_uuid_,
                     smart_device_link_service_uuid_data);
  thread_ = threads::CreateThread("BT Device Scaner",
                                  new  BluetoothDeviceScannerDelegate(this));
}

BluetoothDeviceScanner::~BluetoothDeviceScanner() {
  thread_->join();
  delete thread_->delegate();
  threads::DeleteThread(thread_);
}


bool BluetoothDeviceScanner::IsInitialised() const {
  return thread_ && thread_->is_running();
}

void BluetoothDeviceScanner::UpdateTotalDeviceList() {
  LOG4CXX_AUTO_TRACE(logger_);
  DeviceVector devices;
  devices.insert(devices.end(), paired_devices_with_sdl_.begin(),
                 paired_devices_with_sdl_.end());
  devices.insert(devices.end(), found_devices_with_sdl_.begin(),
                 found_devices_with_sdl_.end());
  controller_->SearchDeviceDone(devices);
}

void BluetoothDeviceScanner::DoInquiry() {
  LOG4CXX_AUTO_TRACE(logger_);

  const int device_id = hci_get_route(0);
  if (device_id < 0) {
    LOG4CXX_INFO(logger_, "HCI device is not available");
    shutdown_requested_ = true;
    controller_->SearchDeviceFailed(SearchDeviceError());
    return;
  }

  int device_handle = hci_open_dev(device_id);
  if (device_handle < 0) {
    controller_->SearchDeviceFailed(SearchDeviceError());
    return;
  }

  if (paired_devices_.empty()) {
    LOG4CXX_INFO(logger_, "Searching for paired devices.");
    if (-1 == FindPairedDevs(&paired_devices_)) {
      LOG4CXX_ERROR(logger_, "Failed to retrieve list of paired devices.");
      controller_->SearchDeviceFailed(SearchDeviceError());
    }
  }

  LOG4CXX_INFO(logger_, "Check rfcomm channel on "
               << paired_devices_.size() << " paired devices.");

  paired_devices_with_sdl_.clear();
  CheckSDLServiceOnDevices(paired_devices_, device_handle,
                           &paired_devices_with_sdl_);
  UpdateTotalDeviceList();

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
    std::vector < bdaddr_t > found_devices(number_of_devices);
    for (int i = 0; i < number_of_devices; ++i) {
      found_devices[i] = inquiry_info_list[i].bdaddr;
    }
    found_devices_with_sdl_.clear();
    CheckSDLServiceOnDevices(found_devices, device_handle,
                             &found_devices_with_sdl_);
  }
  UpdateTotalDeviceList();
  controller_->FindNewApplicationsRequest();

  close(device_handle);
  delete[] inquiry_info_list;

  if (number_of_devices < 0) {
    LOG4CXX_DEBUG(logger_, "number_of_devices < 0");
    controller_->SearchDeviceFailed(SearchDeviceError());
  }
}

void BluetoothDeviceScanner::CheckSDLServiceOnDevices(
  const std::vector<bdaddr_t>& bd_addresses, int device_handle,
  DeviceVector* discovered_devices) {
  LOG4CXX_TRACE(logger_, "enter. bd_addresses: " << &bd_addresses << ", device_handle: " <<
                device_handle << ", discovered_devices: " << discovered_devices);
  std::vector<RfcommChannelVector> sdl_rfcomm_channels =
    DiscoverSmartDeviceLinkRFCOMMChannels(bd_addresses);

  for (size_t i = 0; i < bd_addresses.size(); ++i) {
    if (sdl_rfcomm_channels[i].empty()) {
      continue;
    }

    const bdaddr_t& bd_address = bd_addresses[i];
    char deviceName[256];
    int hci_read_remote_name_ret = hci_read_remote_name(
                                     device_handle, &bd_address, sizeof(deviceName) / sizeof(deviceName[0]),
                                     deviceName, 0);

    if (hci_read_remote_name_ret != 0) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "hci_read_remote_name failed");
      strncpy(deviceName,
              BluetoothDevice::GetUniqueDeviceId(bd_address).c_str(),
              sizeof(deviceName) / sizeof(deviceName[0]));
    }

    Device* bluetooth_device = new BluetoothDevice(bd_address, deviceName,
        sdl_rfcomm_channels[i]);
    if (bluetooth_device) {
      LOG4CXX_INFO(logger_, "Bluetooth device created successfully");
      discovered_devices->push_back(bluetooth_device);
    } else {
      LOG4CXX_WARN(logger_, "Can't create bluetooth device " << deviceName);
    }
  }
  LOG4CXX_TRACE(logger_, "exit");
}

std::vector<BluetoothDeviceScanner::RfcommChannelVector>
BluetoothDeviceScanner::DiscoverSmartDeviceLinkRFCOMMChannels(
  const std::vector<bdaddr_t>& device_addresses) {
  LOG4CXX_TRACE(logger_, "enter device_addresses: " << &device_addresses);
  const size_t size = device_addresses.size();
  std::vector<RfcommChannelVector> result(size);

  static const int attempts = 4;
  static const int attempt_timeout = 5;
  std::vector<bool> processed(size, false);
  unsigned processed_count = 0;
  for (int nattempt = 0; nattempt < attempts; ++nattempt) {
    for (size_t i = 0; i < size; ++i) {
      if (processed[i]) {
        continue;
      }
      const bool final = DiscoverSmartDeviceLinkRFCOMMChannels(
                           device_addresses[i], &result[i]);
      if (final) {
        processed[i] = true;
        ++processed_count;
      }
    }
    if (++processed_count >= size) {
      break;
    }
    sleep(attempt_timeout);
  }
  LOG4CXX_TRACE(logger_, "exit with vector<RfcommChannelVector>: size = " << result.size());
  return result;
}

bool BluetoothDeviceScanner::DiscoverSmartDeviceLinkRFCOMMChannels(
  const bdaddr_t& device_address, RfcommChannelVector* channels) {
  LOG4CXX_TRACE(logger_, "enter. device_address: " << &device_address << ", channels: " <<
                channels);
  static bdaddr_t any_address = { { 0, 0, 0, 0, 0, 0 } };

  sdp_session_t* sdp_session = sdp_connect(
                                 &any_address, &device_address, SDP_RETRY_IF_BUSY | SDP_WAIT_ON_CLOSE);
  if (sdp_session == 0) {
    bool result = !(errno == 31 || errno == 16 || errno == 117 || errno == 114);
    if (result) {
      LOG4CXX_TRACE(logger_, "exit with TRUE. Condition: sdp_session == 0");
    } else {
      LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: sdp_session == 0");
    }
    return result;
  }

  sdp_list_t* search_list = sdp_list_append(0,
                            &smart_device_link_service_uuid_);
  uint32_t range = 0x0000ffff;
  sdp_list_t* attr_list = sdp_list_append(0, &range);
  sdp_list_t* response_list = 0;

  if (0 == sdp_service_search_attr_req(sdp_session, search_list,
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

            for (sdp_data_t* d = sdpData; 0 != d; d = d->next) {
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

  if (!channels->empty()) {
    LOG4CXX_INFO(logger_, "channels not empty");
    std::stringstream rfcomm_channels_string;

    for (RfcommChannelVector::const_iterator it = channels->begin();
         it != channels->end(); ++it) {
      if (it != channels->begin()) {
        rfcomm_channels_string << ", ";
      }
      rfcomm_channels_string << static_cast<uint32_t>(*it);
    }

    LOG4CXX_INFO(logger_,
                 "SmartDeviceLink service was discovered on device "
                 << BluetoothDevice::GetUniqueDeviceId(device_address)
                 << " at channel(s): " << rfcomm_channels_string.str().c_str());
  } else {
    LOG4CXX_INFO(logger_,
                 "SmartDeviceLink service was not discovered on device "
                 << BluetoothDevice::GetUniqueDeviceId(device_address));
  }
  LOG4CXX_TRACE(logger_, "exit with TRUE");
  return true;
}

void BluetoothDeviceScanner::Thread() {
  LOG4CXX_AUTO_TRACE(logger_);
  ready_ = true;
  if (auto_repeat_search_) {
    while (!shutdown_requested_) {
      DoInquiry();
      device_scan_requested_ = false;
      TimedWaitForDeviceScanRequest();
    }
  } else {  // search only on demand
    while (true) {
      {
        sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
        while (!(device_scan_requested_ || shutdown_requested_)) {
          device_scan_requested_cv_.Wait(auto_lock);
        }
      }
      if (shutdown_requested_) {
        break;
      }
      DoInquiry();
      device_scan_requested_ = false;
    }
  }
}

void BluetoothDeviceScanner::TimedWaitForDeviceScanRequest() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (auto_repeat_pause_sec_ == 0) {
    LOG4CXX_TRACE(logger_, "exit. Condition: auto_repeat_pause_sec_ == 0");
    return;
  }

  {
    sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
    while (!(device_scan_requested_ || shutdown_requested_)) {
      const sync_primitives::ConditionalVariable::WaitStatus wait_status =
        device_scan_requested_cv_.WaitFor(auto_lock, auto_repeat_pause_sec_ * 1000);
      if (wait_status == sync_primitives::ConditionalVariable::kTimeout) {
        LOG4CXX_INFO(logger_, "Bluetooth scanner timeout, performing scan");
        device_scan_requested_ = true;
      }
    }
  }
}

TransportAdapter::Error BluetoothDeviceScanner::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  if(!thread_->start()) {
    LOG4CXX_ERROR(logger_, "Bluetooth device scanner thread start failed");
    return TransportAdapter::FAIL;
  }
  LOG4CXX_INFO(logger_, "Bluetooth device scanner thread started");
  return TransportAdapter::OK;
}

void BluetoothDeviceScanner::Terminate() {
  LOG4CXX_AUTO_TRACE(logger_);
  shutdown_requested_ = true;
  if (thread_) {
    {
      sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
      device_scan_requested_ = false;
      device_scan_requested_cv_.NotifyOne();
    }
    LOG4CXX_INFO(logger_,
                 "Waiting for bluetooth device scanner thread termination");
    thread_->stop();
    LOG4CXX_INFO(logger_, "Bluetooth device scanner thread stopped");
  }
}

TransportAdapter::Error BluetoothDeviceScanner::Scan() {
  LOG4CXX_AUTO_TRACE(logger_);
  if ((!IsInitialised()) || shutdown_requested_) {
    LOG4CXX_WARN(logger_, "BAD_STATE");
    return TransportAdapter::BAD_STATE;
  }
  if (auto_repeat_pause_sec_ == 0) {
    return TransportAdapter::OK;
  }
  TransportAdapter::Error ret = TransportAdapter::OK;

  sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
  if (!device_scan_requested_) {
    LOG4CXX_TRACE(logger_, "Requesting device Scan");
    device_scan_requested_ = true;
    device_scan_requested_cv_.NotifyOne();
  } else {
    ret = TransportAdapter::BAD_STATE;
    LOG4CXX_WARN(logger_, "BAD_STATE");
  }
  return ret;
}

BluetoothDeviceScanner::BluetoothDeviceScannerDelegate::BluetoothDeviceScannerDelegate(
    BluetoothDeviceScanner* scanner)
  : scanner_(scanner) {
}

void BluetoothDeviceScanner::BluetoothDeviceScannerDelegate::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(scanner_);
  scanner_->Thread();
}

}  // namespace transport_adapter
}  // namespace transport_manager

