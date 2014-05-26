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
#ifdef MME_MQ
  , event_mqd_(-1)
  , ack_mqd_(-1)
#else
  , mme_hdl_(0)
#endif
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

#ifdef MME_MQ
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
#else
  const std::string& mme_name = profile::Profile::instance()->mme_sync_name();
  LOG4CXX_TRACE(logger_, "Connecting to " << mme_name);
  mme_hdl_ = mme_connect(mme_name.c_str(), 0);
  if (mme_hdl_ != 0) {
    LOG4CXX_DEBUG(logger_, "Connected to " << mme_name);

    notify_thread_ = new threads::Thread("MME notifier", new NotifyThreadDelegate(mme_hdl_, this));
    notify_thread_->start();
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not connect to " << mme_name);
    error = TransportAdapter::FAIL;
  }
#endif

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
#ifdef MME_MQ
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
#else
  const std::string& mme_name = profile::Profile::instance()->mme_sync_name();
  LOG4CXX_TRACE(logger_, "Disconnecting from " << mme_name);
  if (mme_disconnect(mme_hdl_) != -1) {
    LOG4CXX_DEBUG(logger_, "Disconnected from " << mme_name);
  }
  else {
    LOG4CXX_WARN(logger_, "Could not disconnect from " << mme_name);
  }
#endif

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

void MmeDeviceScanner::OnDeviceArrived(msid_t msid) {
  std::string mount_point;
  MmeDevice::Protocol protocol;
  std::string unique_device_id;
  std::string vendor;
  std::string product;
  bool attached; // not used
  if (GetMmeInfo(msid, mount_point, protocol, unique_device_id, vendor, product, attached)) {
    std::string device_name = vendor + " " + product;
    switch (protocol) {
      case MmeDevice::IAP: {
        MmeDevicePtr mme_device(new IAPDevice(mount_point, device_name, unique_device_id, controller_));
        devices_lock_.Acquire();
        devices_.insert(std::make_pair(msid, mme_device));
        devices_lock_.Release();
        NotifyDevicesUpdated();
        break;
      }
      case MmeDevice::IAP2: {
        MmeDevicePtr mme_device(new IAP2Device(mount_point, device_name, unique_device_id, controller_));
        devices_lock_.Acquire();
        devices_.insert(std::make_pair(msid, mme_device));
        devices_lock_.Release();
        NotifyDevicesUpdated();
        break;
      }
      case MmeDevice::UnknownProtocol:
        LOG4CXX_WARN(logger_, "Unsupported protocol for device " << device_name);
        break;
    }
  }
}

void MmeDeviceScanner::OnDeviceLeft(msid_t msid) {
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
    char* data = (char*) qdb_cell(res, 0, 0); // mountpath
    mount_point = std::string(data);
    LOG4CXX_DEBUG(logger_, "Mount point " << mount_point);
    data = (char*) qdb_cell(res, 0, 1); // fs_type
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
    data = (char*) qdb_cell(res, 0, 2); // serial
    unique_device_id = std::string(data);
    LOG4CXX_DEBUG(logger_, "Device ID " << unique_device_id);
    data = (char*) qdb_cell(res, 0, 3); // manufacturer
    vendor = std::string(data);
    LOG4CXX_DEBUG(logger_, "Vendor " << vendor);
    data = (char*) qdb_cell(res, 0, 4); // device_name
    if (strcmp(data, "") != 0) {
      product = std::string(data);
      LOG4CXX_DEBUG(logger_, "Product " << product);
    }
    else { // for some reason device_name can be empty
      product = "Unnamed device";
      LOG4CXX_WARN(logger_, "Unnamed product");
    }
    qdb_int* attached_data = (qdb_int*) qdb_cell(res, 0, 5); // attached
    qdb_int attached_int = *attached_data;
    attached = (attached_int != 0);
    if (attached) {
      LOG4CXX_DEBUG(logger_, "Device is attached");
    }
    else {
      LOG4CXX_DEBUG(logger_, "Device isn\'t attached");
    }
    qdb_freeresult(res);
    return true;
  }
  else {
    LOG4CXX_ERROR(logger_, "Error querying " << mme_db_name);
    return false;
  }
}

#ifdef MME_MQ
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
          LOG4CXX_DEBUG(logger_, "SDL_MSG_IPOD_DEVICE_CONNECT: msid = " << msid << ", name = " << name);
          parent_->OnDeviceArrived(msid);
          LOG4CXX_DEBUG(logger_, "Sending SDL_MSG_IPOD_DEVICE_CONNECT_ACK");
          ack_buffer_[0] = SDL_MSG_IPOD_DEVICE_CONNECT_ACK;
          LOG4CXX_TRACE(logger_, "Sending message to " << ack_mq_name);
          if (mq_send(ack_mqd_, ack_buffer_, kAckBufferSize, 0) != -1) {
            LOG4CXX_DEBUG(logger_, "Message sent to " << ack_mq_name);
          }
          else {
            LOG4CXX_WARN(logger_, "Error occured while sending message to " << ack_mq_name << ", errno = " << errno);
          }
          break;
        }
        case SDL_MSG_IPOD_DEVICE_DISCONNECT: {
          MmeDeviceInfo* mme_device_info = (MmeDeviceInfo*) (&buffer_[1]);
          msid_t msid = mme_device_info->msid;
          const char* name = mme_device_info->name;
          LOG4CXX_DEBUG(logger_, "SDL_MSG_IPOD_DEVICE_DISCONNECT: msid = " << msid << ", name = " << name);
          parent_->OnDeviceLeft(msid);
          LOG4CXX_DEBUG(logger_, "Sending SDL_MSG_IPOD_DEVICE_DISCONNECT_ACK");
          ack_buffer_[0] = SDL_MSG_IPOD_DEVICE_DISCONNECT_ACK;
          LOG4CXX_TRACE(logger_, "Sending message to " << ack_mq_name);
          if (mq_send(ack_mqd_, ack_buffer_, kAckBufferSize, 0) != -1) {
            LOG4CXX_DEBUG(logger_, "Message sent to " << ack_mq_name);
          }
          else {
            LOG4CXX_WARN(logger_, "Error occured while sending message to " << ack_mq_name << ", errno = " << errno);
          }
          break;
        }
      }
    }
    else {
      LOG4CXX_WARN(logger_, "Error occured while receiving message from " << event_mq_name << ", errno = " << errno);
    }
  }
}
#else
MmeDeviceScanner::NotifyThreadDelegate::NotifyThreadDelegate(mme_hdl_t* mme_hdl, MmeDeviceScanner* parent) : parent_(parent), mme_hdl_(mme_hdl) {
}

bool MmeDeviceScanner::NotifyThreadDelegate::ArmEvent(sigevent* event) {
  LOG4CXX_TRACE(logger_, "Arming for MME event notification");
  if (mme_register_for_events(mme_hdl_, MME_EVENT_CLASS_SYNC, event) != -1) {
    LOG4CXX_DEBUG(logger_, "Successfully armed for MME event notification");
    return true;
  }
  else {
    LOG4CXX_WARN(logger_, "Could not arm for MME event notification");
    return false;
  }
}

void MmeDeviceScanner::NotifyThreadDelegate::OnPulse() {
  mme_event_t* mme_event;
  LOG4CXX_TRACE(logger_, "Getting MME event");
  if (mme_get_event(mme_hdl_, &mme_event) != -1) {
    LOG4CXX_DEBUG(logger_, "Parsing MME event");
    switch (mme_event->type) {
      case MME_EVENT_MS_STATECHANGE: {
        mme_ms_statechange_t* mme_ms_statechange = (mme_ms_statechange_t*) (mme_event->data);
        msid_t msid = mme_ms_statechange->msid;
        uint32_t old_state = mme_ms_statechange->old_state;
        uint32_t new_state = mme_ms_statechange->new_state;
        LOG4CXX_DEBUG(logger_, "MME event MME_EVENT_MS_STATECHANGE: msid = " << msid << ", old_state = " << old_state << ", new_state = " << new_state);
// Can't obtain any on documentation on state codes, these are empirical
        if (3 == old_state) {
          parent_->OnDeviceLeft(msid);
        }
        if (3 == new_state) {
          parent_->OnDeviceArrived(msid);
        }
        break;
      }
      default:
        LOG4CXX_DEBUG(logger_, "Not an event of interest");
    }
  }
  else {
    LOG4CXX_WARN(logger_, "Could not get MME event");
  }
}
#endif
}  // namespace transport_adapter
}  // namespace transport_manager
