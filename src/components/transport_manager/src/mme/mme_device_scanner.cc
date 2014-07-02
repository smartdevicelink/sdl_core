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

#include <string.h>

#include "utils/logger.h"
#include "config_profile/profile.h"

#include "transport_manager/mme/mme_device_scanner.h"
#include "transport_manager/mme/iap_device.h"
#include "transport_manager/mme/iap2_device.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

MmeDeviceScanner::MmeDeviceScanner(TransportAdapterController* controller)
  : controller_(controller)
  , initialised_(false)
  , event_mqd_(-1)
  , ack_mqd_(-1)
  , qdb_hdl_(0)
{
}

MmeDeviceScanner::~MmeDeviceScanner() {
}

TransportAdapter::Error MmeDeviceScanner::Init() {
  TransportAdapter::Error error = TransportAdapter::OK;

  const std::string& mme_db_name = profile::Profile::instance()->mme_db_name();
  LOG4CXX_TRACE(logger_, "Connecting to " << mme_db_name);
  qdb_hdl_ = qdb_connect(mme_db_name.c_str(), 0);
  if (qdb_hdl_ != 0) {
    LOG4CXX_DEBUG(logger_, "Connected to " << mme_db_name);
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not connect to " << mme_db_name);
    error = TransportAdapter::FAIL;
  }

  const std::string& event_mq_name = profile::Profile::instance()->event_mq_name();
  const std::string& ack_mq_name = profile::Profile::instance()->ack_mq_name();
#define CREATE_MME_MQ 0
#if CREATE_MME_MQ
  int flags = O_RDONLY | O_CREAT;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  struct mq_attr attributes;
  attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
  attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;
  attributes.mq_flags = 0;
#else
  int flags = O_RDONLY;
#endif
  LOG4CXX_TRACE(logger_, "Opening " << event_mq_name);
#if CREATE_MME_MQ
  event_mqd_ = mq_open(event_mq_name.c_str(), flags, mode, &attributes);
#else
  event_mqd_ = mq_open(event_mq_name.c_str(), flags);
#endif
  if (event_mqd_ != -1) {
    LOG4CXX_DEBUG(logger_, "Opened " << event_mq_name);
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not open " << event_mq_name << ", errno = " << errno);
    error = TransportAdapter::FAIL;
  }
#if CREATE_MME_MQ
  flags = O_WRONLY | O_CREAT;
#else
  flags = O_WRONLY;
#endif
  LOG4CXX_TRACE(logger_, "Opening " << ack_mq_name);
#if CREATE_MME_MQ
  ack_mqd_ = mq_open(ack_mq_name.c_str(), flags, mode, &attributes);
#else
  ack_mqd_ = mq_open(ack_mq_name.c_str(), flags);
#endif
  if (ack_mqd_ != -1) {
    LOG4CXX_DEBUG(logger_, "Opened " << ack_mq_name);
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not open " << ack_mq_name << ", errno = " << errno);
    error = TransportAdapter::FAIL;
  }
  if ((event_mqd_ != -1) && (ack_mqd_ != -1)) {
    notify_thread_ = new threads::Thread("MME MQ notifier", new NotifyThreadDelegate(event_mqd_, ack_mqd_, this));
    notify_thread_->start();
  }

  initialised_ = true;
  return error;
}

TransportAdapter::Error MmeDeviceScanner::Scan() {
  MsidContainer msids;
  if (GetMmeList(msids)) {
    DeviceContainer devices;
    for (MsidContainer::const_iterator i = msids.begin(); i != msids.end(); ++i) {
      msid_t msid = *i;
      std::string mount_point;
      MmeDevice::Protocol protocol;
      std::string unique_device_id;
      std::string vendor;
      std::string product;
      bool attached;
      if (GetMmeInfo(msid, mount_point, protocol, unique_device_id, vendor, product, attached)) {
        if (attached) {
          std::string device_name = vendor + " " + product;
          switch (protocol) {
            case MmeDevice::IAP: {
              MmeDevicePtr mme_device(new IAPDevice(mount_point, device_name, unique_device_id, controller_));
              devices.insert(std::make_pair(msid, mme_device));
              break;
            }
            case MmeDevice::IAP2: {
              MmeDevicePtr mme_device(new IAP2Device(mount_point, device_name, unique_device_id, controller_));
              devices.insert(std::make_pair(msid, mme_device));
              break;
            }
            case MmeDevice::UnknownProtocol:
              LOG4CXX_WARN(logger_, "Unsupported protocol for device " << device_name);
              break;
          }
        }
      }
    }
    devices_lock_.Acquire();
    devices_.swap(devices);
    devices_lock_.Release();

    NotifyDevicesUpdated();

// new device must be initialized
// after ON_SEARCH_DEVICE_DONE notification
// because ON_APPLICATION_LIST_UPDATED event can occur immediately
// which doesn't make sense until device list is updated
    devices_lock_.Acquire();
    for (DeviceContainer::const_iterator i = devices_.begin(); i != devices_.end(); ++i) {
      MmeDevicePtr mme_device = i->second;
      mme_device->Init();
    }
    devices_lock_.Release();

    return TransportAdapter::OK;
  }
  else {
    return TransportAdapter::FAIL;
  }
}

void MmeDeviceScanner::Terminate() {
  if (notify_thread_) {
    notify_thread_->stop();
  }

  const std::string& event_mq_name = profile::Profile::instance()->event_mq_name();
  LOG4CXX_TRACE(logger_, "Closing " << event_mq_name);
  if (mq_close(event_mqd_) != -1) {
    LOG4CXX_DEBUG(logger_, "Closed " << event_mq_name);
  }
  else {
    LOG4CXX_WARN(logger_, "Could not close " << event_mq_name);
  }
  const std::string& ack_mq_name = profile::Profile::instance()->ack_mq_name();
  LOG4CXX_TRACE(logger_, "Closing " << ack_mq_name);
  if (mq_close(ack_mqd_) != -1) {
    LOG4CXX_DEBUG(logger_, "Closed " << ack_mq_name);
  }
  else {
    LOG4CXX_WARN(logger_, "Could not close " << ack_mq_name);
  }

  const std::string& mme_db_name = profile::Profile::instance()->mme_db_name();
  LOG4CXX_TRACE(logger_, "Disconnecting from " << mme_db_name);
  if (qdb_disconnect(qdb_hdl_) != -1) {
    LOG4CXX_DEBUG(logger_, "Disconnected from " << mme_db_name);
  }
  else {
    LOG4CXX_WARN(logger_, "Could not disconnect from " << mme_db_name);
  }
}

bool MmeDeviceScanner::IsInitialised() const {
  return initialised_;
}

void MmeDeviceScanner::OnDeviceArrived(const MmeDeviceInfo* mme_device_info) {
  msid_t msid = mme_device_info->msid;
  std::string mount_point;
  MmeDevice::Protocol protocol;
  std::string unique_device_id;
  std::string vendor;
  std::string product;
  bool attached; // not used
  if (GetMmeInfo(msid, mount_point, protocol, unique_device_id, vendor, product, attached)) {
#define CONSTRUCT_DEVICE_NAME 0
#if CONSTRUCT_DEVICE_NAME
    std::string device_name = vendor + " " + product;
#else
    std::string device_name = std::string(mme_device_info->name);
#endif
    switch (protocol) {
      case MmeDevice::IAP: {
        MmeDevicePtr mme_device(new IAPDevice(mount_point, device_name, unique_device_id, controller_));
        devices_lock_.Acquire();
        devices_.insert(std::make_pair(msid, mme_device));
        devices_lock_.Release();
        NotifyDevicesUpdated();
// new device must be initialized
// after ON_SEARCH_DEVICE_DONE notification
// because ON_APPLICATION_LIST_UPDATED event can occur immediately
// which doesn't make sense until device list is updated
        mme_device->Init();
        break;
      }
      case MmeDevice::IAP2: {
        MmeDevicePtr mme_device(new IAP2Device(mount_point, device_name, unique_device_id, controller_));
        devices_lock_.Acquire();
        devices_.insert(std::make_pair(msid, mme_device));
        devices_lock_.Release();
        NotifyDevicesUpdated();
// new device must be initialized
// after ON_SEARCH_DEVICE_DONE notification
// because ON_APPLICATION_LIST_UPDATED event can occur immediately
// which doesn't make sense until device list is updated
        mme_device->Init();
        break;
      }
      case MmeDevice::UnknownProtocol:
        LOG4CXX_WARN(logger_, "Unsupported protocol for device " << device_name);
        break;
    }
  }
}

void MmeDeviceScanner::OnDeviceLeft(const MmeDeviceInfo* mme_device_info) {
  msid_t msid = mme_device_info->msid;
  bool erased;
  devices_lock_.Acquire();
  DeviceContainer::iterator i = devices_.find(msid);
  if (i != devices_.end()) {
    MmeDevicePtr mme_device = i->second;
    DeviceUID device_id = mme_device->unique_device_id();
    controller_->DeviceDisconnected(device_id, DisconnectDeviceError());
    devices_.erase(i);
    erased = true;
  }
  else {
    erased = false;
  }
  devices_lock_.Release();
  if (erased) {
    NotifyDevicesUpdated();
  }
  else {
    LOG4CXX_WARN(logger_, "Cannot remove device with msid = " << msid);
  }
}

void MmeDeviceScanner::NotifyDevicesUpdated() {
  DeviceVector devices;
  devices_lock_.Acquire();
  for (DeviceContainer::const_iterator i = devices_.begin(); i != devices_.end(); ++i) {
    MmeDevicePtr mme_device = i->second;
    DeviceSptr device = MmeDevicePtr::static_pointer_cast<Device>(mme_device);
    devices.push_back(device);
  }
  devices_lock_.Release();
  controller_->SearchDeviceDone(devices);
}

bool MmeDeviceScanner::GetMmeList(MsidContainer& msids) {
  const char query[] = "SELECT msid FROM mediastores";
  const std::string& mme_db_name = profile::Profile::instance()->mme_db_name();
  LOG4CXX_TRACE(logger_, "Querying " << mme_db_name);
  qdb_result_t* res = qdb_query(qdb_hdl_, 0, query);
  if (res != 0) {
    LOG4CXX_DEBUG(logger_, "Parsing result");
    msids.clear();
    int count = qdb_rows(res);
    for (int i = 0; i < count; ++i) {
      msid_t* data = (msid_t*) qdb_cell(res, i, 0);
      msid_t msid = *data;
      msids.push_back(msid);
    }
    qdb_freeresult(res);
    return true;
  }
  else {
    LOG4CXX_ERROR(logger_, "Error querying " << mme_db_name);
    return false;
  }
}

bool MmeDeviceScanner::GetMmeInfo(
  msid_t msid,
  std::string& mount_point,
  MmeDevice::Protocol& protocol,
  std::string& unique_device_id,
  std::string& vendor,
  std::string& product,
  bool& attached
) {

  const char query[] = "SELECT mountpath, fs_type, serial, manufacturer, device_name, attached FROM mediastores WHERE msid=%lld";
  const std::string& mme_db_name = profile::Profile::instance()->mme_db_name();
  LOG4CXX_TRACE(logger_, "Querying " << mme_db_name);
  qdb_result_t* res = qdb_query(qdb_hdl_, 0, query, msid);
  if (res != 0) {
    LOG4CXX_DEBUG(logger_, "Parsing result");
    bool errors_occurred = false;
    char* data = (char*) qdb_cell(res, 0, 0); // mountpath
    if (data != 0) {
      mount_point = std::string(data);
      LOG4CXX_DEBUG(logger_, "Mount point " << mount_point);
    }
    else {
      LOG4CXX_ERROR(logger_, "Error parsing column mountpath");
      errors_occurred = true;
    }
    data = (char*) qdb_cell(res, 0, 1); // fs_type
    if (data != 0) {
      if (0 == strcmp(data, "ipod")) {
        protocol = MmeDevice::IAP;
        LOG4CXX_DEBUG(logger_, "Protocol iAP");
      }
      else if (0 == strcmp(data, "iap2")) {
        protocol = MmeDevice::IAP2;
        LOG4CXX_DEBUG(logger_, "Protocol iAP2");
      }
      else {
        protocol = MmeDevice::UnknownProtocol;
        LOG4CXX_WARN(logger_, "Unsupported protocol " << data);
      }
    }
    else {
      LOG4CXX_ERROR(logger_, "Error parsing column fs_type");
      errors_occurred = true;
    }
    data = (char*) qdb_cell(res, 0, 2); // serial
    if (data != 0) {
      unique_device_id = std::string(data);
      LOG4CXX_DEBUG(logger_, "Device ID " << unique_device_id);
    }
    else {
      LOG4CXX_ERROR(logger_, "Error parsing column serial");
      errors_occurred = true;
    }
    data = (char*) qdb_cell(res, 0, 3); // manufacturer
    if (data != 0) {
      vendor = std::string(data);
      LOG4CXX_DEBUG(logger_, "Vendor " << vendor);
    }
    else {
      LOG4CXX_ERROR(logger_, "Error parsing column manufacturer");
      errors_occurred = true;
    }
    data = (char*) qdb_cell(res, 0, 4); // device_name
    if (data != 0) {
      product = std::string(data);
      LOG4CXX_DEBUG(logger_, "Product " << product);
    }
    else {
      LOG4CXX_ERROR(logger_, "Error parsing column device_name");
      errors_occurred = true;
    }
    qdb_int* attached_data = (qdb_int*) qdb_cell(res, 0, 5); // attached
    if (attached_data != 0) {
      qdb_int attached_int = *attached_data;
      attached = (attached_int != 0);
      if (attached) {
        LOG4CXX_DEBUG(logger_, "Device is attached");
      }
      else {
        LOG4CXX_DEBUG(logger_, "Device isn\'t attached");
      }
    }
    else {
      LOG4CXX_ERROR(logger_, "Error parsing column attached");
      errors_occurred = true;
    }
    qdb_freeresult(res);
    return !errors_occurred;
  }
  else {
    LOG4CXX_ERROR(logger_, "Error querying " << mme_db_name);
    return false;
  }
}

MmeDeviceScanner::NotifyThreadDelegate::NotifyThreadDelegate(mqd_t event_mqd, mqd_t ack_mqd, MmeDeviceScanner* parent) : parent_(parent), event_mqd_(event_mqd), ack_mqd_(ack_mqd), run_(true) {
}

void MmeDeviceScanner::NotifyThreadDelegate::threadMain() {
  const std::string& event_mq_name = profile::Profile::instance()->event_mq_name();
  const std::string& ack_mq_name = profile::Profile::instance()->ack_mq_name();
  while (run_) {
    LOG4CXX_TRACE(logger_, "Waiting for message from " << event_mq_name);
    ssize_t size = mq_receive(event_mqd_, buffer_, kBufferSize, 0);
    if (size != -1) {
      LOG4CXX_DEBUG(logger_, "Received " << size << " bytes from " << event_mq_name);
      char code = buffer_[0];
      LOG4CXX_DEBUG(logger_, "code = " << (int) code);
      switch (code) {
        case SDL_MSG_IPOD_DEVICE_CONNECT: {
          MmeDeviceInfo* mme_device_info = (MmeDeviceInfo*) (&buffer_[1]);
          msid_t msid = mme_device_info->msid;
          const char* name = mme_device_info->name;
          const char* protocol = mme_device_info->iAP2 ? "iAP2" : "iAP";
          LOG4CXX_DEBUG(logger_, "SDL_MSG_IPOD_DEVICE_CONNECT: msid = " << msid
            << ", name = " << name << ", protocol = " << protocol);
          parent_->OnDeviceArrived(mme_device_info);
          LOG4CXX_DEBUG(logger_, "Sending SDL_MSG_IPOD_DEVICE_CONNECT_ACK");
          ack_buffer_[0] = SDL_MSG_IPOD_DEVICE_CONNECT_ACK;
          LOG4CXX_TRACE(logger_, "Sending message to " << ack_mq_name);
          if (mq_send(ack_mqd_, ack_buffer_, kAckBufferSize, 0) != -1) {
            LOG4CXX_DEBUG(logger_, "Message sent to " << ack_mq_name);
          }
          else {
            LOG4CXX_WARN(logger_, "Error occurred while sending message to " << ack_mq_name << ", errno = " << errno);
          }
          break;
        }
        case SDL_MSG_IPOD_DEVICE_DISCONNECT: {
          MmeDeviceInfo* mme_device_info = (MmeDeviceInfo*) (&buffer_[1]);
          msid_t msid = mme_device_info->msid;
          const char* name = mme_device_info->name;
          const char* protocol = mme_device_info->iAP2 ? "iAP2" : "iAP";
          LOG4CXX_DEBUG(logger_, "SDL_MSG_IPOD_DEVICE_DISCONNECT: msid = " << msid
            << ", name = " << name << ", protocol = " << protocol);
          parent_->OnDeviceLeft(mme_device_info);
          LOG4CXX_DEBUG(logger_, "Sending SDL_MSG_IPOD_DEVICE_DISCONNECT_ACK");
          ack_buffer_[0] = SDL_MSG_IPOD_DEVICE_DISCONNECT_ACK;
          LOG4CXX_TRACE(logger_, "Sending message to " << ack_mq_name);
          if (mq_send(ack_mqd_, ack_buffer_, kAckBufferSize, 0) != -1) {
            LOG4CXX_DEBUG(logger_, "Message sent to " << ack_mq_name);
          }
          else {
            LOG4CXX_WARN(logger_, "Error occurred while sending message to " << ack_mq_name << ", errno = " << errno);
          }
          break;
        }
      }
    }
    else {
      LOG4CXX_WARN(logger_, "Error occurred while receiving message from " << event_mq_name << ", errno = " << errno);
    }
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
