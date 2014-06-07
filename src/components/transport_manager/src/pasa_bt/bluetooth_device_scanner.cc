/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "transport_manager/pasa_bt/bluetooth_device_scanner.h"

//#include <bluetooth/bluetooth.h>
//#include <bluetooth/hci.h>
//#include <bluetooth/hci_lib.h>
//#include <bluetooth/sdp.h>
//#include <bluetooth/sdp_lib.h>
//#include <bluetooth/rfcomm.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <sstream>
#include "transport_manager/pasa_bt/bluetooth_transport_adapter.h"
#include "transport_manager/pasa_bt/bluetooth_device.h"

#include "utils/logger.h"

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

// Todd: BT Support
#if 0
int FindPairedDevs(std::vector<bdaddr_t>* result) {
  DCHECK(result != NULL);

  const char* cmd = "bt-device -l";

  FILE* pipe = popen(cmd, "r");
  if (!pipe) {
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
  return 0;
}
#endif

}  //  namespace

BluetoothDeviceScanner::BluetoothDeviceScanner(
  TransportAdapterController* controller, bool auto_repeat_search,
  int auto_repeat_pause_sec)
  : controller_(controller),
    thread_(),
    thread_started_(false),
    shutdown_requested_(false),
    ready_(true),
    device_scan_requested_(false),
    device_scan_requested_lock_(),
    device_scan_requested_cv_(),
    auto_repeat_search_(auto_repeat_search),
// Todd: BT Support
    mPASAFWSendHandle(openMsgQ(PREFIX_STR_FROMSDLCOREBTADAPTER_QUEUE, true, true)),
    mIncomingDevice(NULL),
    auto_repeat_pause_sec_(auto_repeat_pause_sec) {
// Todd: BT support
#if 0
  uint8_t smart_device_link_service_uuid_data[] = { 0x93, 0x6D, 0xA0, 0x1F,
      0x9A, 0xBD, 0x4D, 0x9D, 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8
                                                  };
  sdp_uuid128_create(&smart_device_link_service_uuid_,
                     smart_device_link_service_uuid_data);
#endif
  pthread_mutex_init(&devices_mutex_, 0);
}

BluetoothDeviceScanner::~BluetoothDeviceScanner() {
  pthread_mutex_destroy(&devices_mutex_);
}

// Todd: BT support
mqd_t BluetoothDeviceScanner::openMsgQ(const char *queue_name, bool sender, bool block)
{
    struct mq_attr attributes;
    attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
    attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;
    attributes.mq_flags = 0;
    int flags = 0;

    if (sender)
    {
    	flags = (block) ? (O_WRONLY|O_CREAT) : (O_WRONLY|O_CREAT|O_NONBLOCK);
    }
    else
    {
    	flags = (block) ? (O_RDONLY|O_CREAT) : (O_RDONLY|O_CREAT|O_NONBLOCK);
    }

   return mq_open(queue_name, flags, 0666, &attributes);
}

// Todd: BT support
void* BluetoothDeviceScanner::handlePASAFrameworkIncomingMessages(void *data)
{
    LOG4CXX_TRACE_ENTER(logger_);
    BluetoothDeviceScanner* bluetoothDeviceScanner =
      static_cast<BluetoothDeviceScanner*>(data);

    bluetoothDeviceScanner->mq_ToSDL = bluetoothDeviceScanner->openMsgQ(PREFIX_STR_TOSDLCOREBTADAPTER_QUEUE, false, true);

    char buffer[MAX_QUEUE_MSG_SIZE];
    ssize_t length=0;

    while (!bluetoothDeviceScanner->shutdown_requested_)
    {
        if ((length = mq_receive(bluetoothDeviceScanner->mq_ToSDL, buffer, sizeof(buffer), 0)) != -1)
        {

        	switch (buffer[0])
        	{
				case SDL_MSG_BT_DEVICE_CONNECT:
				{
                    LOG4CXX_INFO(logger_, "Received PASA FW BT SPP Connect Message");
					bluetoothDeviceScanner->connectBTDevice(&buffer[1]);
				}
				break;
				case SDL_MSG_BT_DEVICE_SPP_DISCONNECT:
				{
                    LOG4CXX_INFO(logger_, "Received PASA FW BT SPP Disconnect Message");
					bluetoothDeviceScanner->disconnectBTDeviceSPP(&buffer[1]);
				}
				break;
				case SDL_MSG_BT_DEVICE_DISCONNECT:
				{
                    LOG4CXX_INFO(logger_, "Received PASA FW BT Disconnect Message");
					bluetoothDeviceScanner->disconnectBTDevice(&buffer[1]);
				}
				break;
                case SDL_MSG_BT_DEVICE_CONNECT_END:
                {
                    LOG4CXX_INFO(logger_, "Received PASA FW BT SPP END Message");
                	bluetoothDeviceScanner->UpdateTotalDeviceList();
                	bluetoothDeviceScanner->UpdateTotalApplicationList();
                }
                break;

				default:
                    LOG4CXX_INFO(logger_, "Received PASA FW BT Unknown Message");
				break;
        	}
        }
    }
    LOG4CXX_TRACE_EXIT(logger_);

	return (NULL);
}



void* bluetoothDeviceScannerThread(void* data) {
  LOG4CXX_TRACE_ENTER(logger_);
  BluetoothDeviceScanner* bluetoothDeviceScanner =
    static_cast<BluetoothDeviceScanner*>(data);
  assert(bluetoothDeviceScanner != 0);
  bluetoothDeviceScanner->Thread();
  LOG4CXX_TRACE_EXIT(logger_);
  return 0;
}

bool BluetoothDeviceScanner::IsInitialised() const {
  return thread_started_;
}

void BluetoothDeviceScanner::UpdateTotalDeviceList() {
  LOG4CXX_TRACE_ENTER(logger_);
  DeviceVector devices;

// Todd: BT Support
//  devices.insert(devices.end(), paired_devices_with_sdl_.begin(),
//                 paired_devices_with_sdl_.end());
  pthread_mutex_lock(&devices_mutex_);
  devices.insert(devices.end(), found_devices_with_sdl_.begin(),
                 found_devices_with_sdl_.end());
  pthread_mutex_unlock(&devices_mutex_);
  controller_->SearchDeviceDone(devices);
  LOG4CXX_TRACE_EXIT(logger_);
}

void BluetoothDeviceScanner::UpdateTotalApplicationList() {
  LOG4CXX_TRACE_ENTER(logger_);
  controller_->FindNewApplicationsRequest();
  LOG4CXX_TRACE_EXIT(logger_);
}

void BluetoothDeviceScanner::DoInquiry() {
  LOG4CXX_TRACE_ENTER(logger_);

// Todd: BT support 
#if 0
  const int device_id = hci_get_route(0);
  if (device_id < 0) {
    LOG4CXX_INFO(logger_, "HCI device is not available");
    shutdown_requested_ = true;
    controller_->SearchDeviceFailed(SearchDeviceError());
    return;
  }

  int device_handle = hci_open_dev(device_id);
  if (device_handle < 0) {
    LOG4CXX_INFO(logger_, "device_handle < 0, exit DoInquiry");
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
#endif

// Todd: BT support
#if 0
  close(device_handle);
  delete[] inquiry_info_list;

  if (number_of_devices < 0) {
    LOG4CXX_INFO(logger_, "number_of_devices < 0");
    LOG4CXX_TRACE_EXIT(logger_);
    controller_->SearchDeviceFailed(SearchDeviceError());
  }

  LOG4CXX_INFO(logger_, "Bluetooth inquiry finished");
  LOG4CXX_TRACE_EXIT(logger_);
#endif

}

// Todd: BT support 
int BluetoothDeviceScanner::sendMsg(mqd_t q_fd, uint8_t msgType, uint32_t length, const void *data)
{
	uint8_t sndBuf[MSGQ_MESSAGE_SIZE] = { 0 };

	if(length > 0 && NULL == data)
	{
		return -1;
	}

	sndBuf[0] = msgType;

	if ( length > 0 && data != NULL )
	{
		// copy the msg to send buffer
		memcpy(&sndBuf[0 + sizeof(msgType)], data, length);
	}

	return mq_send(q_fd, (const char *)&sndBuf[0], sizeof(sndBuf), 0);
}

// Todd: BT support 
void BluetoothDeviceScanner::connectBTDevice(void *data)
{
	PBTDeviceConnectInfo pDeviceInfo = static_cast<PBTDeviceConnectInfo>(data);

	pthread_mutex_lock(&devices_mutex_);

	BluetoothDevice* device = NULL;
	for (DeviceVector::const_iterator i = found_devices_with_sdl_.begin(); i != found_devices_with_sdl_.end(); ++i) {
		BluetoothDevice* existing_device = static_cast<BluetoothDevice*>(i->get());
		if (0 == memcmp(existing_device->mac(), pDeviceInfo->mac, sizeof(pDeviceInfo->mac))) {
            LOG4CXX_INFO(logger_, "Bluetooth device exists: " << pDeviceInfo->cDeviceName);
		    device = existing_device;
			break;
		}
	}

	if (device == NULL) {
		device = new BluetoothDevice(pDeviceInfo->cDeviceName, pDeviceInfo->mac);
        LOG4CXX_INFO(logger_, "Bluetooth device created successfully: " << pDeviceInfo->cDeviceName);
        found_devices_with_sdl_.push_back(device);
	}
	pthread_mutex_unlock(&devices_mutex_);

	if (device) {
		BluetoothDevice::SCOMMChannel tChannel(pDeviceInfo->cSppQueName);
		device->AddChannel(tChannel);
        LOG4CXX_INFO(logger_, "Bluetooth channel " << pDeviceInfo->cSppQueName << " added to " << pDeviceInfo->cDeviceName << " " << device);
	}

	sendMsg(mPASAFWSendHandle, SDL_MSG_BT_DEVICE_CONNECT_ACK, 0, NULL);
}

// Todd: BT support 
void BluetoothDeviceScanner::disconnectBTDevice(void *data)
{
	PBTDeviceDisconnectInfo pDeviceInfo = static_cast<PBTDeviceDisconnectInfo>(data);
	DeviceUID device_id = MacToString(pDeviceInfo->mac);
    controller_->DisconnectDevice(device_id);
    sendMsg(mPASAFWSendHandle, SDL_MSG_BT_DEVICE_DISCONNECT_ACK, 0, NULL);
}

void BluetoothDeviceScanner::disconnectBTDeviceSPP(void *data)
{
	PBTDeviceDisConnectSPPInfo pDeviceInfo = static_cast<PBTDeviceDisConnectSPPInfo>(data);

	pthread_mutex_lock(&devices_mutex_);
	for (DeviceVector::iterator i = found_devices_with_sdl_.begin(); i != found_devices_with_sdl_.end(); ++i)
	{
		BluetoothDevice* device = static_cast<BluetoothDevice*>(i->get());
		if (0 == memcmp(device->mac(), pDeviceInfo->mac, sizeof(pDeviceInfo->mac))) {
			BluetoothDevice::SCOMMChannel tChannel(pDeviceInfo->cSppQueName);
			ApplicationHandle disconnected_app;
			const bool found = device->RemoveChannel(tChannel, &disconnected_app);
			if (found)
				controller_->AbortConnection(device->unique_device_id(), disconnected_app);
			break;
		}
	}
	pthread_mutex_unlock(&devices_mutex_);
}





// Todd: BT Support
#if 0
void BluetoothDeviceScanner::CheckSDLServiceOnDevices(
  const std::vector<bdaddr_t>& bd_addresses, int device_handle,
  DeviceVector* discovered_devices) {
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
}
#endif


// Todd: BT Support
#if 0
std::vector<BluetoothDeviceScanner::RfcommChannelVector> BluetoothDeviceScanner::DiscoverSmartDeviceLinkRFCOMMChannels(
  const std::vector<bdaddr_t>& device_addresses) {
  LOG4CXX_TRACE_ENTER(logger_);
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
  return result;
}
#endif

// Todd: BT Support
#if 0
bool BluetoothDeviceScanner::DiscoverSmartDeviceLinkRFCOMMChannels(
  const bdaddr_t& device_address, RfcommChannelVector* channels) {
  static bdaddr_t any_address = { { 0, 0, 0, 0, 0, 0 } };

  sdp_session_t* sdp_session = sdp_connect(
                                 &any_address, &device_address, SDP_RETRY_IF_BUSY | SDP_WAIT_ON_CLOSE);
  if (sdp_session == 0) {
    LOG4CXX_INFO(logger_, "sdp_connect errno " << errno);
    return !(errno == 31 || errno == 16 || errno == 117 || errno == 114);
  }

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

    LOG4CXX_INFO(
      logger_,
      "SmartDeviceLink service was discovered on device "
      << BluetoothDevice::GetUniqueDeviceId(device_address)
      << " at channel(s): " << rfcomm_channels_string.str().c_str());
  } else {
    LOG4CXX_INFO(
      logger_,
      "SmartDeviceLink service was not discovered on device "
      << BluetoothDevice::GetUniqueDeviceId(device_address));
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}
#endif


void BluetoothDeviceScanner::Thread() {
  LOG4CXX_TRACE_ENTER(logger_);
  LOG4CXX_INFO(logger_, "Bluetooth adapter main thread initialized");
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

  LOG4CXX_INFO(logger_, "Bluetooth device scanner thread finished");
  LOG4CXX_TRACE_EXIT(logger_);
}

void BluetoothDeviceScanner::TimedWaitForDeviceScanRequest() {
  LOG4CXX_TRACE_ENTER(logger_);

  if (auto_repeat_pause_sec_ == 0) {
    LOG4CXX_TRACE_EXIT(logger_);
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

  LOG4CXX_TRACE_EXIT(logger_);
}

TransportAdapter::Error BluetoothDeviceScanner::Init() {
  LOG4CXX_TRACE_ENTER(logger_);

// Todd: Support BT 
  int thread_start_error;

  found_devices_with_sdl_.clear();

  thread_start_error = pthread_create(&mPASAFWMsgHandlerThread, 0, handlePASAFrameworkIncomingMessages, this);

  if (0 == thread_start_error) {

      pthread_setname_np(mPASAFWMsgHandlerThread, "PASAFW BTAdapter");

      thread_start_error = pthread_create(&thread_, 0,
                                     &bluetoothDeviceScannerThread,
                                     this);
  }

  if (0 == thread_start_error) {
    thread_started_ = true;
    LOG4CXX_INFO(logger_, "Bluetooth device scanner thread started");
  } else {
    LOG4CXX_ERROR(
      logger_,
      "Bluetooth device scanner thread start failed, error code "
      << thread_start_error);
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return TransportAdapter::OK;
}

void BluetoothDeviceScanner::Terminate() {
  LOG4CXX_TRACE_ENTER(logger_);
  shutdown_requested_ = true;
  mq_close(mq_ToSDL);

  if (true == thread_started_) {
    {
      sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
    device_scan_requested_ = false;
      device_scan_requested_cv_.NotifyOne();
    }
    LOG4CXX_INFO(logger_,
                 "Waiting for bluetooth device scanner thread termination");

// Todd: BT Support
    pthread_join(mPASAFWMsgHandlerThread, 0);

    pthread_join(thread_, 0);
    LOG4CXX_INFO(logger_, "Bluetooth device scanner thread terminated");
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

TransportAdapter::Error BluetoothDeviceScanner::Scan() {
  LOG4CXX_TRACE_ENTER(logger_);
  if ((!thread_started_) || shutdown_requested_) {
    LOG4CXX_INFO(logger_, "bad state");
    return TransportAdapter::BAD_STATE;
  }

// Todd: BT Support
//  if (auto_repeat_pause_sec_ == 0) {
//    LOG4CXX_INFO(logger_, "no search pause, scan forcing not needed");
//    return TransportAdapter::OK;
//  }
  TransportAdapter::Error ret = TransportAdapter::OK;

  {
    sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
  if (false == device_scan_requested_) {
    LOG4CXX_INFO(logger_, "Requesting device Scan");
    device_scan_requested_ = true;
      device_scan_requested_cv_.NotifyOne();
  } else {
    ret = TransportAdapter::BAD_STATE;
    LOG4CXX_INFO(logger_, "Device Scan is currently in progress");
  }
  }

  LOG4CXX_TRACE_EXIT(logger_);
  return ret;
}

}  // namespace transport_adapter
}  // namespace transport_manager

