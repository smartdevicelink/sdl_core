/**
 * \file platform_usb_device.cc
 * \brief QNX PlatformUsbDevice class source file.
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

#include "transport_manager/usb/qnx/platform_usb_device.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

PlatformUsbDevice::PlatformUsbDevice(
    usbd_device_instance_t* instance, usbd_device* device,
    const usbd_device_descriptor_t& device_descriptor)
    : bus_number_(instance->path),
      address_(instance->devno),
      vendor_id_(instance->ident.vendor),
      product_id_(instance->ident.device),
      device_descriptor_(device_descriptor),
      usbd_device_instance_(*instance),
      usbd_device_(device) {}

std::string PlatformUsbDevice::GetDescString(uint8_t index) const {
  char* str = usbd_string(usbd_device_, index, 0);
  if (NULL == str) {
    LOG4CXX_INFO(logger_, "Failed to get USB string descriptor");
    return "";
  }
  return std::string(str);
}

std::string PlatformUsbDevice::GetManufacturer() const {
  return GetDescString(device_descriptor_.iManufacturer);
}

std::string PlatformUsbDevice::GetProductName() const {
  return GetDescString(device_descriptor_.iProduct);
}

std::string PlatformUsbDevice::GetSerialNumber() const {
  return GetDescString(device_descriptor_.iSerialNumber);
}

}  // namespace
}  // namespace
