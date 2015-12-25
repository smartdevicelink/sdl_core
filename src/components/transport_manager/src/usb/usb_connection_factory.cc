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

#include "transport_manager/usb/usb_connection_factory.h"
#include "transport_manager/usb/usb_device.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "utils/logger.h"

#if defined(__QNXNTO__)
#include "transport_manager/usb/qnx/usb_connection.h"
#else
#include "transport_manager/usb/libusb/usb_connection.h"
#endif

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

UsbConnectionFactory::UsbConnectionFactory(
  TransportAdapterController* controller)
  : controller_(controller), usb_handler_() {}

TransportAdapter::Error UsbConnectionFactory::Init() {
  return TransportAdapter::OK;
}

void UsbConnectionFactory::SetUsbHandler(const UsbHandlerSptr usb_handler) {
  usb_handler_ = usb_handler;
}

TransportAdapter::Error UsbConnectionFactory::CreateConnection(
  const DeviceUID& device_uid, const ApplicationHandle& app_handle) {
  LOG4CXX_TRACE(logger_, "enter DeviceUID: " << &device_uid << ", ApplicationHandle: " <<
                &app_handle);
  DeviceSptr device = controller_->FindDevice(device_uid);
  if (!device.valid()) {
    LOG4CXX_ERROR(logger_, "device " << device_uid << " not found");
    LOG4CXX_TRACE(logger_,
                  "exit with TransportAdapter::BAD_PARAM. Condition: !device.valid()");
    return TransportAdapter::BAD_PARAM;
  }

  UsbDevice* usb_device = static_cast<UsbDevice*>(device.get());
  UsbConnection* usb_connection =
    new UsbConnection(device_uid, app_handle, controller_, usb_handler_,
                      usb_device->usb_device());

  controller_->ConnectionCreated(usb_connection, device_uid, app_handle);

  if (usb_connection->Init()) {
    LOG4CXX_INFO(logger_, "USB connection initialised");
    LOG4CXX_TRACE(logger_, "exit with TransportAdapter::OK. Condition: USB connection initialised");
    return TransportAdapter::OK;
  } else {
    LOG4CXX_TRACE(logger_, "exit with TransportAdapter::FAIL. Condition: USB connection NOT initialised");
    return TransportAdapter::FAIL;
  }
}

void UsbConnectionFactory::Terminate() {}

bool UsbConnectionFactory::IsInitialised() const {
  return true;
}

UsbConnectionFactory::~UsbConnectionFactory() {}

}  // namespace transport_adapter
}  // namespace transport_manager
