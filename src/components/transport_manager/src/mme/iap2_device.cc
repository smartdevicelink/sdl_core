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

#include <list>

#include "transport_manager/mme/iap2_device.h"

namespace transport_manager {
namespace transport_adapter {

std::list<std::string> iap2Protocols; // TODO(nvaganov@luxoft.com) choose protocol name
std::string path_to_config_file = "/fs/mp/etc/mm/iap2.cfg";

void fillIap2Protocols() {
    iap2Protocols.push_back("com.ford.sync.prot0");
    iap2Protocols.push_back("com.ford.sync.prot1");
    iap2Protocols.push_back("com.ford.sync.prot2");
    iap2Protocols.push_back("com.ford.sync.prot3");
    iap2Protocols.push_back("com.ford.sync.prot4");
    iap2Protocols.push_back("com.ford.sync.prot5");
    iap2Protocols.push_back("com.ford.sync.prot6");
}

IAP2Device::IAP2Device(const std::string& mount_point,
                       const std::string& name,
                       const DeviceUID& unique_device_id) :
  MmeDevice(mount_point, name, unique_device_id) {
  last_used_app_id_ = 1; // fisrt application id at device
  fillIap2Protocols();
  for (std::list<std::string>::iterator it = iap2Protocols.begin(); it != iap2Protocols.end(); it++) {
      utils::SharedPtr<threads::Thread> thr = new threads::Thread(it->c_str(),
        new iap2_connect_thread(this, *it) );
      thr->start();
      threads_.push_back(thr);
  }
}

ApplicationList IAP2Device::GetApplicationList() const {
    ApplicationList app_list;
    app_list.push_back(1);
    return app_list;
}

void IAP2Device::on_iap2SessionReady(iap2ea_hdl_t* handler) {
    iap2ea_handlers_.insert(std::make_pair(last_used_app_id_++, handler));
}

void IAP2Device::iap2_connect_thread::threadMain() {
  iap2ea_hdl_t* handler;
  handler = iap2_eap_open(path_to_config_file.c_str(), protocol_name_.c_str(), 0);
  if (!handler){
    parent_->on_iap2SessionReady(handler);
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
