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

#include <string>
#include <fstream>

#include "utils/logger.h"
#include "config_profile/profile.h"

#include "transport_manager/mme/iap2_device.h"
#include "transport_manager/mme/protocol_config.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

IAP2Device::IAP2Device(const std::string& mount_point,
                       const std::string& name,
                       const DeviceUID& unique_device_id,
                       TransportAdapterController* controller) :
  MmeDevice(mount_point, name, unique_device_id),
  controller_(controller),
  last_app_id_(0) {
}

IAP2Device::~IAP2Device() {
  for (ThreadContainer::const_iterator i = hub_connection_threads_.begin(); i != hub_connection_threads_.end(); ++i) {
    utils::SharedPtr<threads::Thread> thread = i->second;
    thread->stop();
  }
  for (ThreadContainer::const_iterator i = legacy_connection_threads_.begin(); i != legacy_connection_threads_.end(); ++i) {
    utils::SharedPtr<threads::Thread> thread = i->second;
    thread->stop();
  }
  for (ThreadContainer::const_iterator i = pool_connection_threads_.begin(); i != pool_connection_threads_.end(); ++i) {
    utils::SharedPtr<threads::Thread> thread = i->second;
    thread->stop();
  }
}

bool IAP2Device::Init() {
  const ProtocolConfig::ProtocolNameContainer& legacy_protocol_names = ProtocolConfig::LegacyProtocolNames();
  for (ProtocolConfig::ProtocolNameContainer::const_iterator i = legacy_protocol_names.begin(); i != legacy_protocol_names.end(); ++i) {
    std::string protocol_name = *i;
    ::std::string thread_name = "iAP2 connect notifier (" + protocol_name + ")";
    utils::SharedPtr<threads::Thread> thread = new threads::Thread(thread_name.c_str(),
      new IAP2ConnectThreadDelegate(this, protocol_name));
    LOG4CXX_INFO(logger_, "iAP2: starting connection thread for legacy protocol " << protocol_name);
    thread->start();
    legacy_connection_threads_.insert(std::make_pair(protocol_name, thread));
  }

  int pool_index = 0;
  const ProtocolConfig::ProtocolNameContainer& pool_protocol_names = ProtocolConfig::IAP2PoolProtocolNames();
  for (ProtocolConfig::ProtocolNameContainer::const_iterator i = pool_protocol_names.begin(); i != pool_protocol_names.end(); ++i) {
    std::string protocol_name = *i;
    free_protocol_name_pool_.insert(std::make_pair(++pool_index, protocol_name));
  }

  const ProtocolConfig::ProtocolNameContainer& hub_protocol_names = ProtocolConfig::HubProtocolNames();
  for (ProtocolConfig::ProtocolNameContainer::const_iterator i = hub_protocol_names.begin(); i != hub_protocol_names.end(); ++i) {
    std::string protocol_name = *i;
    ::std::string thread_name = "iAP2 hub connect notifier (" + protocol_name + ")";
    utils::SharedPtr<threads::Thread> thread = new threads::Thread(thread_name.c_str(),
      new IAP2HubConnectThreadDelegate(this, protocol_name));
    LOG4CXX_INFO(logger_, "iAP2: starting connection thread for hub protocol " << protocol_name);
    thread->start();
    hub_connection_threads_.insert(std::make_pair(protocol_name, thread));
  }

  return true;
}

ApplicationList IAP2Device::GetApplicationList() const {
  ApplicationList app_list;
  apps_lock_.Acquire();
  for (AppContainer::const_iterator i = apps_.begin(); i != apps_.end(); ++i) {
    ApplicationHandle app_id = i->first;
    app_list.push_back(app_id);
  }
  apps_lock_.Release();
  return app_list;
}

bool IAP2Device::RecordByAppId(ApplicationHandle app_id, AppRecord& record) const {
  sync_primitives::AutoLock auto_lock(apps_lock_);
  AppContainer::const_iterator i = apps_.find(app_id);
  if (i != apps_.end()) {
    record = i->second;
    return true;
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: no record corresponding to application " << app_id);
    return false;
  }
}

void IAP2Device::OnHubConnect(const std::string& protocol_name, iap2ea_hdl_t* handle) {
  char protocol_index;
  std::string pool_protocol_name;
  bool thread_launched;
  protocol_name_pool_lock_.Acquire();
  if (!free_protocol_name_pool_.empty()) {
    FreeProtocolNamePool::iterator i = free_protocol_name_pool_.begin();
    protocol_index = i->first;
    pool_protocol_name = i->second;
    std::string thread_name = "iAP2 connect notifier (" + pool_protocol_name + ")";
    utils::SharedPtr<threads::Thread> thread = new threads::Thread(thread_name.c_str(),
      new IAP2ConnectThreadDelegate(this, pool_protocol_name));
    LOG4CXX_INFO(logger_, "iAP2: starting connection thread for protocol " << pool_protocol_name);
    thread->start();
    thread_launched = true;
    pool_connection_threads_lock_.Acquire();
    ThreadContainer::iterator j = pool_connection_threads_.find(pool_protocol_name);
    if (j != pool_connection_threads_.end()) {
      pool_connection_threads_.erase(j);
    }
    pool_connection_threads_.insert(std::make_pair(pool_protocol_name, thread));
    pool_connection_threads_lock_.Release();
    protocol_in_use_name_pool_.insert(std::make_pair(pool_protocol_name, protocol_index));
    free_protocol_name_pool_.erase(i);
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: protocol pool is empty");
    protocol_index = 255;
    thread_launched = false;
  }
  protocol_name_pool_lock_.Release();
  char buffer[] = {protocol_index};
  LOG4CXX_TRACE(logger_, "iAP2: sending data on hub protocol " << protocol_name);
  if (iap2_eap_send(handle, buffer, sizeof(buffer)) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP2: data on hub protocol " << protocol_name << " sent successfully");
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: error occurred while sending data on hub protocol " << protocol_name);
    if (thread_launched) {
      pool_connection_threads_lock_.Acquire();
      ThreadContainer::iterator j = pool_connection_threads_.find(pool_protocol_name);
      if (j != pool_connection_threads_.end()) {
        utils::SharedPtr<threads::Thread> thread = j->second;
        thread->stop();
        pool_connection_threads_.erase(j);
      }
      pool_connection_threads_lock_.Release();
      ReturnToPool(pool_protocol_name);
    }
  }
  LOG4CXX_TRACE(logger_, "iAP2: closing connection on hub protocol " << protocol_name);
  if (iap2_eap_close(handle) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP2: connection on hub protocol " << protocol_name << " closed");
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: could not close connection on hub protocol " << protocol_name);
  }
}

void IAP2Device::OnConnect(const std::string& protocol_name, iap2ea_hdl_t* handle) {
  apps_lock_.Acquire();
  ApplicationHandle app_id = ++last_app_id_;
  AppRecord record = std::make_pair(protocol_name, handle);
  apps_.insert(std::make_pair(app_id, record));
  apps_lock_.Release();

  controller_->ApplicationListUpdated(unique_device_id());
}

void IAP2Device::OnConnectFailed(const std::string& protocol_name) {
  ReturnToPool(protocol_name);
}

void IAP2Device::OnDisconnect(ApplicationHandle app_id) {
  bool removed = false;
  apps_lock_.Acquire();
  AppContainer::iterator i = apps_.find(app_id);
  if (i != apps_.end()) {
    AppRecord record = i->second;
    std::string protocol_name = record.first;
    LOG4CXX_DEBUG(logger_, "iAP2: dropping protocol " << protocol_name << " for application " << app_id);
    apps_.erase(i);
    removed = true;
    ThreadContainer::const_iterator j = legacy_connection_threads_.find(protocol_name);
    if (j != legacy_connection_threads_.end()) {
      utils::SharedPtr<threads::Thread> thread = j->second;
      LOG4CXX_INFO(logger_, "iAP2: restarting connection thread for legacy protocol " << protocol_name);
      thread->start();
    }
    else {
      if (!ReturnToPool(protocol_name)) {
        LOG4CXX_WARN(logger_, "iAP2: protocol " << protocol_name << " is neither legacy protocol nor pool protocol in use");
      }
    }
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: no protocol corresponding to application " << app_id);
  }
  apps_lock_.Release();

  if (removed) {
    controller_->ApplicationListUpdated(unique_device_id());
  }
}

bool IAP2Device::ReturnToPool(const std::string& protocol_name) {
  sync_primitives::AutoLock auto_lock(protocol_name_pool_lock_);
  ProtocolInUseNamePool::iterator i = protocol_in_use_name_pool_.find(protocol_name);
  if (i != protocol_in_use_name_pool_.end()) {
    LOG4CXX_INFO(logger_, "iAP2: returning protocol " << protocol_name << " back to pool");
    int protocol_index = i->second;
    free_protocol_name_pool_.insert(std::make_pair(protocol_index, protocol_name));
    protocol_in_use_name_pool_.erase(i);
    return true;
  }
  else {
    return false;
  }
}

IAP2Device::IAP2HubConnectThreadDelegate::IAP2HubConnectThreadDelegate(
  IAP2Device* parent,
  const std::string& protocol_name): parent_(parent),
  protocol_name_(protocol_name) {
}

void IAP2Device::IAP2HubConnectThreadDelegate::threadMain() {
  std::string mount_point = parent_->mount_point();
  int max_attempts = profile::Profile::instance()->iap2_hub_connect_attempts();
  int attemtps = 0;
  while (true) {
    LOG4CXX_TRACE(logger_, "iAP2: connecting to " << mount_point << " on hub protocol " << protocol_name_);
    iap2ea_hdl_t* handle = iap2_eap_open(mount_point.c_str(), protocol_name_.c_str(), 0);
    if (handle != 0) {
      LOG4CXX_DEBUG(logger_, "iAP2: connected to " << mount_point << " on hub protocol " << protocol_name_);
      attemtps = 0;
      parent_->OnHubConnect(protocol_name_, handle);
    }
    else {
      if ((0 == max_attempts) || (++attemtps < max_attempts)) {
        LOG4CXX_WARN(logger_, "iAP2: could not connect to " << mount_point << " on hub protocol " << protocol_name_);
      }
      else {
        LOG4CXX_ERROR(logger_, "iAP2: hub protocol " << protocol_name_ << " unavailable after " << max_attempts << " attempts in a row, quit trying");
        break;
      }
    }
  }
}

IAP2Device::IAP2ConnectThreadDelegate::IAP2ConnectThreadDelegate(
  IAP2Device* parent,
  const std::string& protocol_name): parent_(parent),
  protocol_name_(protocol_name) {
}

void IAP2Device::IAP2ConnectThreadDelegate::threadMain() {
  std::string mount_point = parent_->mount_point();
  LOG4CXX_TRACE(logger_, "iAP2: connecting to " << mount_point << " on protocol " << protocol_name_);
  iap2ea_hdl_t* handle = iap2_eap_open(mount_point.c_str(), protocol_name_.c_str(), 0);
  if (handle != 0) {
    LOG4CXX_DEBUG(logger_, "iAP2: connected to " << mount_point << " on protocol " << protocol_name_);
    parent_->OnConnect(protocol_name_, handle);
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: could not connect to " << mount_point << " on protocol " << protocol_name_);
    parent_->OnConnectFailed(protocol_name_);
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
