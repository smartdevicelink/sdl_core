/**
 * \file usb_aoa_adapter.cpp
 * \brief UsbAoaAdapter class source file.
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

#include "transport_manager/usb/usb_aoa_adapter.h"
#include "transport_manager/usb/usb_device_scanner.h"
#include "transport_manager/usb/usb_connection_factory.h"
#include "transport_manager/usb/common.h"
#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")
UsbAoaAdapter::UsbAoaAdapter()
  : TransportAdapterImpl(new UsbDeviceScanner(this),
                         new UsbConnectionFactory(this), 0),
  is_initialised_(false),
  usb_handler_(new UsbHandler()) {
  static_cast<UsbDeviceScanner*>(device_scanner_)->SetUsbHandler(usb_handler_);
  static_cast<UsbConnectionFactory*>(server_connection_factory_)
  ->SetUsbHandler(usb_handler_);
}

UsbAoaAdapter::~UsbAoaAdapter() {}

DeviceType UsbAoaAdapter::GetDeviceType() const {
  return "sdl-usb-aoa";
}

bool UsbAoaAdapter::IsInitialised() const {
  return is_initialised_ && TransportAdapterImpl::IsInitialised();
}

TransportAdapter::Error UsbAoaAdapter::Init() {
  LOG4CXX_TRACE(logger_, "enter");
  TransportAdapter::Error error = usb_handler_->Init();
  if (error != TransportAdapter::OK) {
    LOG4CXX_TRACE(logger_, "exit with error " << error <<
                  ". Condition: error != TransportAdapter::OK");
    return error;
  }
  error = TransportAdapterImpl::Init();
  if (error != TransportAdapter::OK) {
    LOG4CXX_TRACE(logger_, "exit with error " << error <<
                  ". Condition: error != TransportAdapter::OK");
    return error;
  }
  is_initialised_ = true;
  LOG4CXX_TRACE(logger_, "exit with TransportAdapter::OK");
  return TransportAdapter::OK;
}

bool UsbAoaAdapter::ToBeAutoConnected(DeviceSptr device) const {
  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager
