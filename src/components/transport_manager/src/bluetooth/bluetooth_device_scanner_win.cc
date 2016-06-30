/*
 * \file bluetooth_device_scanner_win.cc
 * \brief BluetoothDeviceScanner class header file.
 *
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

#include "transport_manager/bluetooth/bluetooth_device_scanner.h"

#include <stdlib.h>
#include <sys/types.h>
#include <io.h>
#include <BaseTsd.h>
#include <fcntl.h>

#include <vector>
#include <sstream>
#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"
#include "transport_manager/bluetooth/bluetooth_device.h"
#include "utils/bluetooth/bluetooth_service_record_win.h"
#include "utils/bluetooth/bluetooth_uuid_win.h"

#include "utils/logger.h"
#include "utils/threads/thread.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOGGER("TransportManager")

BluetoothDeviceScanner::BluetoothDeviceScanner(
    TransportAdapterController* controller,
    bool auto_repeat_search,
    int auto_repeat_pause_sec)
    : controller_(controller)
    , thread_(NULL)
    , shutdown_requested_(false)
    , ready_(true)
    , device_scan_requested_(false)
    , device_scan_requested_lock_()
    , device_scan_requested_cv_()
    , auto_repeat_search_(auto_repeat_search)
    , auto_repeat_pause_sec_(auto_repeat_pause_sec) {
  BYTE smart_device_link_service_uuid_data[16];
  smart_device_link_service_uuid_data[0] = 0x93;
  smart_device_link_service_uuid_data[1] = 0x6D;
  smart_device_link_service_uuid_data[2] = 0xA0;
  smart_device_link_service_uuid_data[3] = 0x1F;
  smart_device_link_service_uuid_data[4] = 0x9A;
  smart_device_link_service_uuid_data[5] = 0xBD;
  smart_device_link_service_uuid_data[6] = 0x4D;
  smart_device_link_service_uuid_data[7] = 0x9D;
  smart_device_link_service_uuid_data[8] = 0x80;
  smart_device_link_service_uuid_data[9] = 0xC7;
  smart_device_link_service_uuid_data[10] = 0x02;
  smart_device_link_service_uuid_data[11] = 0xAF;
  smart_device_link_service_uuid_data[12] = 0x85;
  smart_device_link_service_uuid_data[13] = 0xC8;
  smart_device_link_service_uuid_data[14] = 0x22;
  smart_device_link_service_uuid_data[15] = 0xA8;

  utils::ConvertBytesToUUID(smart_device_link_service_uuid_data,
                            smart_device_link_service_uuid_);
  service_uuid_str_ =
      utils::BluetoothUUID(utils::GuidToStr(smart_device_link_service_uuid_))
          .Value();
  thread_ = threads::CreateThread("BT Device Scaner",
                                  new BluetoothDeviceScannerDelegate(this));
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
  SDL_AUTO_TRACE();
  DeviceVector devices;
  devices.insert(devices.end(),
                 paired_devices_with_sdl_.begin(),
                 paired_devices_with_sdl_.end());
  devices.insert(devices.end(),
                 found_devices_with_sdl_.begin(),
                 found_devices_with_sdl_.end());
  controller_->SearchDeviceDone(devices);
}

void BluetoothDeviceScanner::DoInquiry() {
  HANDLE radio_handle;
  BLUETOOTH_FIND_RADIO_PARAMS bluetooth_find_param = {
      sizeof(bluetooth_find_param)};
  bluetooth_find_param.dwSize = sizeof(bluetooth_find_param);
  HBLUETOOTH_RADIO_FIND hdbluetooth_dev_find_res =
      BluetoothFindFirstRadio(&bluetooth_find_param, &radio_handle);

  if (!hdbluetooth_dev_find_res) {
    SDL_ERROR("Failed to find first radio: " << GetLastError());
    return;
  }

  std::vector<BLUETOOTH_DEVICE_INFO> found_devices;
  BLUETOOTH_DEVICE_INFO_STRUCT device_info;
  device_info.dwSize = sizeof(device_info);
  BLUETOOTH_DEVICE_SEARCH_PARAMS device_search_params;
  memset(&device_search_params, 0, sizeof(device_search_params));
  device_search_params.dwSize = sizeof(device_search_params);

  device_search_params.fReturnAuthenticated = true;
  device_search_params.fReturnConnected = true;
  device_search_params.hRadio = radio_handle;

  if (hdbluetooth_dev_find_res) {
    do {
      BLUETOOTH_RADIO_INFO radio_info;
      radio_info.dwSize = sizeof(radio_info);
      if (ERROR_SUCCESS != BluetoothGetRadioInfo(radio_handle, &radio_info)) {
        SDL_WARN("Bluetooth device not found. Error: "
                 << utils::GetLastErrorMessage(GetLastError()));
        return;
      }
      HANDLE device_find =
          BluetoothFindFirstDevice(&device_search_params, &device_info);
      GUID* installed_services = &smart_device_link_service_uuid_;
      DWORD num_services = NULL;
      if (device_find) {
        do {
          BluetoothEnumerateInstalledServices(
              radio_handle, &device_info, &num_services, installed_services);
          if (num_services) {
            found_devices.push_back(device_info);
          }
        } while (BluetoothFindNextDevice(device_find, &device_info));

        BluetoothFindDeviceClose(device_find);
      }
    } while (BluetoothFindNextRadio(hdbluetooth_dev_find_res, &radio_handle));
    CloseHandle(radio_handle);
    BluetoothFindRadioClose(hdbluetooth_dev_find_res);
  }
  found_devices_with_sdl_.clear();
  QueryBthProtocolInfo();
  CheckSDLServiceOnDevices(found_devices,
                           reinterpret_cast<int>(hdbluetooth_dev_find_res),
                           &found_devices_with_sdl_);
  if (found_devices_with_sdl_ != paired_devices_with_sdl_) {
    paired_devices_with_sdl_ = found_devices_with_sdl_;
    UpdateTotalDeviceList();
    controller_->FindNewApplicationsRequest();
  }
  if (found_devices.empty()) {
    SDL_DEBUG("No devices were found");
    controller_->SearchDeviceFailed(SearchDeviceError());
  }
}

void BluetoothDeviceScanner::QueryBthProtocolInfo() {
  SOCKET bth_socket = ::socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
  DWORD lastError = ::GetLastError();
  if (bth_socket == INVALID_SOCKET) {
    SDL_ERROR("Failed to get bluetooth socket! "
              << utils::GetLastErrorMessage(lastError));
    return;
  }
  int protocol_info_size = sizeof(protocol_info_);
  int retVal = ::getsockopt(bth_socket,
                            SOL_SOCKET,
                            SO_PROTOCOL_INFO,
                            (char*)&protocol_info_,
                            &protocol_info_size);
  lastError = ::GetLastError();
  closesocket(bth_socket);
  if (retVal) {
    SDL_ERROR("Failed to get bluetooth socket options! "
              << utils::GetLastErrorMessage(lastError));
  }
}

void BluetoothDeviceScanner::CheckSDLServiceOnDevices(
    const std::vector<BLUETOOTH_DEVICE_INFO>& bd_addresses,
    int device_handle,
    DeviceVector* discovered_devices) {
  std::vector<RfcommChannelVector> sdl_rfcomm_channels =
      DiscoverSmartDeviceLinkRFCOMMChannels(bd_addresses);

  for (size_t i = 0; i < bd_addresses.size(); ++i) {
    if (sdl_rfcomm_channels[i].empty()) {
      continue;
    }

    const BLUETOOTH_DEVICE_INFO bd_address = bd_addresses[i];
    char deviceName[256];
    char servInfo[NI_MAXSERV];
    DWORD hci_read_remote_name_ret =
        getnameinfo((struct sockaddr*)device_handle,
                    sizeof(struct sockaddr),
                    deviceName,
                    NI_MAXHOST,
                    servInfo,
                    NI_MAXSERV,
                    NI_NUMERICSERV);

    if (hci_read_remote_name_ret != 0) {
      SDL_ERROR_WITH_ERRNO("hci_read_remote_name failed");
      strncpy(deviceName,
              BluetoothDevice::GetUniqueDeviceId(bd_address).c_str(),
              sizeof(deviceName) / sizeof(deviceName[0]));
    }

    Device* bluetooth_device = new BluetoothDevice(
        bd_address, deviceName, sdl_rfcomm_channels[i], sock_addr_bth_server_);

    if (bluetooth_device) {
      SDL_INFO("Bluetooth device created successfully");
      discovered_devices->push_back(bluetooth_device);
    } else {
      SDL_WARN("Can't create bluetooth device " << deviceName);
    }
  }
}

std::vector<BluetoothDeviceScanner::RfcommChannelVector>
BluetoothDeviceScanner::DiscoverSmartDeviceLinkRFCOMMChannels(
    const std::vector<BLUETOOTH_DEVICE_INFO>& device_addresses) {
  const size_t size = device_addresses.size();
  std::vector<RfcommChannelVector> result(size);
  sock_addr_bth_server_.addressFamily = NULL;
  sock_addr_bth_server_.btAddr = NULL;
  sock_addr_bth_server_.port = NULL;
  static const int attempts = 4;
  static const int attempt_timeout = 1000;
  std::vector<bool> processed(size, false);
  unsigned processed_count = 0;
  for (int nattempt = 0; nattempt < attempts; ++nattempt) {
    for (size_t i = 0; i < size; ++i) {
      if (processed[i]) {
        continue;
      }
      const bool final = DiscoverSmartDeviceLinkRFCOMMChannels(
          device_addresses[i], &result[i], sock_addr_bth_server_);
      if (final) {
        processed[i] = true;
        ++processed_count;
      }
    }
    if (++processed_count >= size) {
      break;
    }
    Sleep(attempt_timeout);
  }
  return result;
}

bool BluetoothDeviceScanner::DiscoverSmartDeviceLinkRFCOMMChannels(
    const BLUETOOTH_DEVICE_INFO& device_address,
    RfcommChannelVector* channels,
    SOCKADDR_BTH& sock_addr_bth_server) {
  std::string str_device_address = utils::BthDeviceAddrToStr(device_address);
  HANDLE handle_service_search;
  DWORD flags = LUP_FLUSHCACHE | LUP_RETURN_NAME | LUP_RETURN_TYPE |
                LUP_RETURN_ADDR | LUP_RETURN_BLOB | LUP_RETURN_COMMENT |
                LUP_RES_SERVICE;
  WSAQUERYSET service_search_quiery;
  ZeroMemory(&service_search_quiery, sizeof(service_search_quiery));
  service_search_quiery.dwSize = sizeof(service_search_quiery);
  GUID protocol = L2CAP_PROTOCOL_UUID;
  service_search_quiery.lpServiceClassId = &protocol;
  service_search_quiery.dwNameSpace = NS_BTH;
  service_search_quiery.lpszContext =
      const_cast<LPSTR>(str_device_address.c_str());

  int service_scan_result = WSALookupServiceBegin(
      &service_search_quiery, LUP_FLUSHCACHE, &handle_service_search);
  if (!service_scan_result) {
    while (true) {
      BYTE buffer[2000];
      DWORD bufferLength = sizeof(buffer);
      WSAQUERYSET* pResults = reinterpret_cast<WSAQUERYSET*>(&buffer);
      if (WSALookupServiceNext(
              handle_service_search, flags, &bufferLength, pResults)) {
        DWORD error = GetLastError();
        if (WSA_E_NO_MORE != error) {
          SDL_WARN("Service scan error: " << utils::GetLastErrorMessage(error));
        }
        break;
      } else {
        if (pResults->lpBlob) {
          const BLOB pBlob = static_cast<BLOB>(*pResults->lpBlob);
          utils::BluetoothServiceRecordWin BtHServiceParser(
              str_device_address,
              std::string(pResults->lpszServiceInstanceName),
              utils::ByteArrayToVector(pBlob),
              utils::BluetoothUUID(service_uuid_str_));
          if (BtHServiceParser.IsUuidEqual(service_uuid_str_)) {
            ULONGLONG ululRemoteBthAddr = 0u;
            BTH_ADDR* pRemoteBtAddr =
                static_cast<BTH_ADDR*>(&ululRemoteBthAddr);
            CopyMemory(
                pRemoteBtAddr,
                &(reinterpret_cast<PSOCKADDR_BTH>(
                      pResults->lpcsaBuffer->RemoteAddr.lpSockaddr))->btAddr,
                sizeof(*pRemoteBtAddr));
            sock_addr_bth_server.addressFamily = AF_BTH;
            sock_addr_bth_server.btAddr = *pRemoteBtAddr;
            sock_addr_bth_server.serviceClassId =
                smart_device_link_service_uuid_;
            sock_addr_bth_server.port = BtHServiceParser.RfcommChannel();
            channels->push_back(BtHServiceParser.RfcommChannel());
          }
        }
      }
    }
    WSALookupServiceEnd(handle_service_search);
  } else {
    SDL_ERROR(
        "Service search error:" << utils::GetLastErrorMessage(GetLastError()));
    return false;
  }
  return true;
}

void BluetoothDeviceScanner::Thread() {
  SDL_AUTO_TRACE();
  ready_ = true;
  if (auto_repeat_search_) {
    while (!shutdown_requested_) {
      DoInquiry();
      device_scan_requested_ = false;
      TimedWaitForDeviceScanRequest();
    }
  } else {  // search only on demand
    while (!shutdown_requested_) {
      {
        sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
        while (!(device_scan_requested_ || shutdown_requested_)) {
          device_scan_requested_cv_.Wait(auto_lock);
        }
      }
      DoInquiry();
      device_scan_requested_ = false;
    }
  }
}

void BluetoothDeviceScanner::TimedWaitForDeviceScanRequest() {
  if (auto_repeat_pause_sec_ == 0) {
    return;
  }

  {
    sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
    while (!(device_scan_requested_ || shutdown_requested_)) {
      const sync_primitives::ConditionalVariable::WaitStatus wait_status =
          device_scan_requested_cv_.WaitFor(auto_lock,
                                            auto_repeat_pause_sec_ * 1000);
      if (wait_status == sync_primitives::ConditionalVariable::kTimeout) {
        SDL_INFO("Bluetooth scanner timeout, performing scan");
        device_scan_requested_ = true;
      }
    }
  }
}

TransportAdapter::Error BluetoothDeviceScanner::Init() {
  SDL_AUTO_TRACE();
  if (!thread_->start()) {
    SDL_ERROR("Bluetooth device scanner thread start failed");
    return TransportAdapter::FAIL;
  }
  SDL_INFO("Bluetooth device scanner thread started");
  return TransportAdapter::OK;
}

void BluetoothDeviceScanner::Terminate() {
  SDL_AUTO_TRACE();
  shutdown_requested_ = true;
  if (thread_) {
    {
      sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
      device_scan_requested_ = false;
      device_scan_requested_cv_.NotifyOne();
    }
    SDL_INFO("Waiting for bluetooth device scanner thread termination");
    thread_->stop();
    SDL_INFO("Bluetooth device scanner thread stopped");
  }
}

TransportAdapter::Error BluetoothDeviceScanner::Scan() {
  SDL_AUTO_TRACE();
  if ((!IsInitialised()) || shutdown_requested_) {
    SDL_WARN("BAD_STATE");
    return TransportAdapter::BAD_STATE;
  }
  if (auto_repeat_pause_sec_ == 0) {
    return TransportAdapter::OK;
  }
  TransportAdapter::Error ret = TransportAdapter::OK;

  sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
  if (!device_scan_requested_) {
    SDL_TRACE("Requesting device Scan");
    device_scan_requested_ = true;
    device_scan_requested_cv_.NotifyOne();
  } else {
    ret = TransportAdapter::BAD_STATE;
    SDL_WARN("BAD_STATE");
  }
  return ret;
}

BluetoothDeviceScanner::BluetoothDeviceScannerDelegate::
    BluetoothDeviceScannerDelegate(BluetoothDeviceScanner* scanner)
    : scanner_(scanner) {}

void BluetoothDeviceScanner::BluetoothDeviceScannerDelegate::threadMain() {
  SDL_AUTO_TRACE();
  DCHECK(scanner_);
  scanner_->Thread();
}

}  // namespace transport_adapter
}  // namespace transport_manager
