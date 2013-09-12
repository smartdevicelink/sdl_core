/*
 * \file transport_manager_default.cc
 * \brief
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

#include "transport_manager/transport_manager_default.h"

#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"
#include "transport_manager/tcp/tcp_transport_adapter.h"
//#include "transport_manager/usb/usb_aoa_adapter.h"


using transport_manager::transport_adapter::BluetoothTransportAdapter;
using transport_manager::transport_adapter::TcpTransportAdapter;
//using transport_manager::transport_adapter::UsbAoaAdapter;

namespace transport_manager {

int TransportManagerDefault::Init() {
  if (E_SUCCESS != TransportManagerImpl::Init()) {
    return E_TM_IS_NOT_INITIALIZED;
  }

  AddTransportAdapter(bluetooth_da_);
  AddTransportAdapter(tcp_da_);
//  AddTransportAdapter(usb_aoa_da_);

  return E_SUCCESS;
}

TransportManagerDefault::~TransportManagerDefault() {
  if (is_initialized_) {
    RemoveTransportAdapter(bluetooth_da_);
    RemoveTransportAdapter(tcp_da_);
//    RemoveTransportAdapter(usb_aoa_da_);
  }
}

TransportManagerAttr default_config_ = { 0 };

TransportManagerDefault::TransportManagerDefault(
    const TransportManagerAttr& config)
    : TransportManagerImpl(config),
      bluetooth_da_(new BluetoothTransportAdapter()),
      tcp_da_(new TcpTransportAdapter())/*,
      usb_aoa_da_(new UsbAoaAdapter())*/ {}

TransportManagerDefault* TransportManagerDefault::Instance() {
  static pthread_mutex_t tm_default_instance_mutex = PTHREAD_MUTEX_INITIALIZER;
  static TransportManagerDefault* tm_default_instance = nullptr;

  if (tm_default_instance == nullptr) {
    pthread_mutex_lock(&tm_default_instance_mutex);
    if (tm_default_instance == nullptr) {
      tm_default_instance = new TransportManagerDefault(default_config_);
      tm_default_instance->Init();
    }
    pthread_mutex_unlock(&tm_default_instance_mutex);
  }
  return tm_default_instance;
}

}  //  namespace transport_manager
