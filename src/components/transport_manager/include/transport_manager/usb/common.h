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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_COMMON_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_COMMON_H_

#include <stdint.h>

#include "utils/shared_ptr.h"

#if defined(__QNXNTO__)
#include "transport_manager/usb/qnx/usb_handler.h"
#else
#include "transport_manager/usb/libusb/usb_handler.h"
#endif

namespace transport_manager {

namespace transport_adapter {

static const uint16_t kAoaVid = 0x18d1;
static const uint16_t kAoaPid1 = 0x2d00;
static const uint16_t kAoaPid2 = 0x2d01;
static const uint8_t kAoaInterfaceSubclass = 0xff;

static const uint16_t kAppleVid = 0x05ac;
static const uint16_t kApplePid1 = 0x1290; // iPhone
static const uint16_t kApplePid2 = 0x1292; // iPhone 3G
static const uint16_t kApplePid3 = 0x1294; // iPhone 3GS
static const uint16_t kApplePid4 = 0x1297; // iPhone 4
static const uint16_t kApplePid5 = 0x129a; // iPad
static const uint16_t kApplePid6 = 0x129f; // iPad 2
static const uint16_t kApplePid7 = 0x12a0; // iPhone 4S
static const uint16_t kApplePid8 = 0x12a8; // iPhone 5

static const int kUsbConfiguration = 1;

typedef utils::SharedPtr<UsbHandler> UsbHandlerSptr;

class UsbDeviceListener {
 public:
  virtual ~UsbDeviceListener() {}

  UsbHandlerSptr GetUsbHandler() { return usb_handler_; }

  void SetUsbHandler(UsbHandlerSptr usb_handler) {
    usb_handler_ = usb_handler;
    usb_handler_->usb_device_listeners_.push_back(this);
  }

  virtual void OnDeviceArrived(PlatformUsbDevice* device) = 0;
  virtual void OnDeviceLeft(PlatformUsbDevice* device) = 0;

 private:
  UsbHandlerSptr usb_handler_;
};

inline bool IsGoogleAccessory(const PlatformUsbDevice* device) {
  return (kAoaVid == device->vendor_id()) &&
    ((kAoaPid1 == device->product_id()) || (kAoaPid2 == device->product_id()));
}

inline bool IsAppleIAPDevice(const PlatformUsbDevice* device) {
  return (kAppleVid == device->vendor_id()) &&
    ((kApplePid1 == device->product_id()) ||
     (kApplePid2 == device->product_id()) ||
     (kApplePid3 == device->product_id()) ||
     (kApplePid4 == device->product_id()) ||
     (kApplePid5 == device->product_id()) ||
     (kApplePid6 == device->product_id()) ||
     (kApplePid7 == device->product_id()));
}

inline bool IsAppleIAP2Device(const PlatformUsbDevice* device) {
  return (kAppleVid == device->vendor_id()) &&
    (kApplePid8 == device->product_id());
}

inline bool IsAppleDevice(const PlatformUsbDevice* device) {
  return IsAppleIAPDevice(device) || IsAppleIAP2Device(device);
}

}  // namespace
}  // namespace

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_COMMON_H_
