/**
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

#include "transport_manager/usb/libusb/platform_usb_device.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")


PlatformUsbDevice::PlatformUsbDevice(
  uint8_t bus_number, uint8_t address,
  const libusb_device_descriptor& device_descriptor,
  libusb_device* device_libusb, libusb_device_handle* device_handle_libusb)
  : bus_number_(bus_number),
    address_(address),
    vendor_id_(device_descriptor.idVendor),
    product_id_(device_descriptor.idProduct),
    device_descriptor_(device_descriptor),
    libusb_device_handle_(device_handle_libusb),
    libusb_device_(device_libusb) {}

std::string PlatformUsbDevice::GetDescString(uint8_t index) const {
  LOG4CXX_TRACE(logger_, "enter. index: " << int(index));
  unsigned char buf[128];
  const int libusb_ret = libusb_get_string_descriptor_ascii(
                           libusb_device_handle_, index, buf, sizeof(buf));
  if (libusb_ret < 0) {
    LOG4CXX_ERROR(logger_, "Failed to get USB string descriptor: "
                 << libusb_error_name(libusb_ret));
    LOG4CXX_TRACE(logger_, "exit with empty string");
    return "";
  }
  LOG4CXX_TRACE(logger_, "exit");
  return std::string(reinterpret_cast<char*>(buf));
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
