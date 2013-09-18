/**
 * \file usb_device_scanner.cc
 * \brief UsbDeviceScanner class source file.
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

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/usb/usb_device_scanner.h"
#include "transport_manager/usb/usb_device.h"
#include "transport_manager/usb/libusb_handler.h"

namespace transport_manager {
namespace transport_adapter {

class AoaInitSequence : public UsbControlTransferSequence {
 public:
  AoaInitSequence(libusb_device_handle* device_handle);
 private:
  class AoaGetProtocolRequest;
  class AoaSendIdString;
  class AoaTurnIntoAccessoryMode;
};

bool IsGoogleAccessory(const libusb_device_descriptor& descriptor) {
  static const uint16_t AOA_VID = 0x18d1;
  static const uint16_t AOA_PID1 = 0x2d00;
  static const uint16_t AOA_PID2 = 0x2d01;
  return descriptor.idVendor == AOA_VID
      && (descriptor.idProduct == AOA_PID1 || descriptor.idProduct == AOA_PID2);
}

void UsbDeviceScanner::OnDeviceArrived(libusb_device* device) {
  LOG4CXX_INFO(
      logger_,
      "libusb device arrived (bus number " << static_cast<int>(libusb_get_bus_number(device)) << ", address " << static_cast<int>(libusb_get_device_address(device)) << ")");

  libusb_device_descriptor descriptor;
  int libusb_ret = libusb_get_device_descriptor(device, &descriptor);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_,
                  "libusb_get_device_descriptor failed: " << libusb_ret);
    return;
  }

  libusb_device_handle* device_handle;
  libusb_ret = libusb_open(device, &device_handle);
  if (libusb_ret != LIBUSB_SUCCESS) {
    LOG4CXX_ERROR(logger_,
                  "libusb_open failed: " << libusb_error_name(libusb_ret));
    return;
  }

  if (IsGoogleAccessory(descriptor)) {
    GoogleAccessoryFound(device, descriptor, device_handle);
  } else {
    TurnIntoAccessoryMode(descriptor, device_handle);
  }
}

void UsbDeviceScanner::OnDeviceLeft(libusb_device* device) {
  LOG4CXX_INFO(
      logger_,
      "libusb device left (bus number " << static_cast<int>(libusb_get_bus_number(device)) << ", address " << static_cast<int>(libusb_get_device_address(device)) << ")");

  pthread_mutex_lock(&device_descriptions_mutex_);
  for (DeviceDescriptions::iterator it = device_descriptions_.begin();
      it != device_descriptions_.end();) {
    const UsbDeviceDescription& description = *it;
    if (libusb_get_bus_number(device) == description.bus_number
        && libusb_get_device_address(device) == description.address) {
      it = device_descriptions_.erase(it);
    } else {
      ++it;
    }
  }
  pthread_mutex_unlock(&device_descriptions_mutex_);
}

UsbDeviceScanner::UsbDeviceScanner(TransportAdapterController* controller)
    : controller_(controller) {
  pthread_mutex_init(&device_descriptions_mutex_, 0);
}

UsbDeviceScanner::~UsbDeviceScanner() {
  pthread_mutex_destroy(&device_descriptions_mutex_);
}

class AoaInitSequence::AoaGetProtocolRequest : public UsbControlInTransfer {
  virtual ~AoaGetProtocolRequest() {
  }
  virtual libusb_request_type RequestType() const {
    return LIBUSB_REQUEST_TYPE_VENDOR;
  }
  virtual uint8_t Request() const {
    return 51;
  }
  virtual uint16_t Value() const {
    return 0;
  }
  virtual uint16_t Index() const {
    return 0;
  }
  virtual uint16_t Length() const {
    return 2;
  }
  virtual bool OnCompleted(unsigned char* data) const {
    const int protocol_version = data[1] << 8 | data[0];
    LOG4CXX_INFO(logger_, "AOA protocol version " << protocol_version);
    if (protocol_version == 0) {
      // AOA protocol not supported
      return false;
    }
    return true;
  }
};

class AoaInitSequence::AoaSendIdString : public UsbControlOutTransfer {
 public:
  AoaSendIdString(uint16_t index, const char* string, uint16_t length)
      : index_(index),
        string_(string),
        length_(length) {
  }
 private:
  virtual ~AoaSendIdString() {
  }
  virtual libusb_request_type RequestType() const {
    return LIBUSB_REQUEST_TYPE_VENDOR;
  }
  virtual uint8_t Request() const {
    return 52;
  }
  virtual uint16_t Value() const {
    return 0;
  }
  virtual uint16_t Index() const {
    return index_;
  }
  virtual uint16_t Length() const {
    return length_;
  }
  virtual const char* Data() const {
    return string_;
  }
  uint16_t index_;
  const char* string_;
  uint16_t length_;
};

class AoaInitSequence::AoaTurnIntoAccessoryMode : public UsbControlOutTransfer {
  virtual ~AoaTurnIntoAccessoryMode() {
  }
  virtual libusb_request_type RequestType() const {
    return LIBUSB_REQUEST_TYPE_VENDOR;
  }
  virtual uint8_t Request() const {
    return 53;
  }
  virtual uint16_t Value() const {
    return 0;
  }
  virtual uint16_t Index() const {
    return 0;
  }
  virtual uint16_t Length() const {
    return 0;
  }
  virtual const char* Data() const {
    return 0;
  }
};

static char manufacturer[] = "Ford";
static char model_name[] = "HMI";
static char description[] = "Human machine interface";
static char version[] = "1.0";
static char uri[] = "http://www.ford.com";
static char serial_num[] = "N000000";

AoaInitSequence::AoaInitSequence(libusb_device_handle* device_handle)
    : UsbControlTransferSequence(device_handle) {
  AddTransfer(new AoaGetProtocolRequest);
  AddTransfer(new AoaSendIdString(0, manufacturer, sizeof(manufacturer)));
  AddTransfer(new AoaSendIdString(1, model_name, sizeof(model_name)));
  AddTransfer(new AoaSendIdString(2, description, sizeof(description)));
  AddTransfer(new AoaSendIdString(3, version, sizeof(version)));
  AddTransfer(new AoaSendIdString(4, uri, sizeof(uri)));
  AddTransfer(new AoaSendIdString(5, serial_num, sizeof(serial_num)));
  AddTransfer(new AoaTurnIntoAccessoryMode);
}

void UsbDeviceScanner::TurnIntoAccessoryMode(
    const libusb_device_descriptor& descriptor,
    libusb_device_handle* device_handle) {
  LOG4CXX_INFO(
      logger_,
      "USB device descriptor VID:" << descriptor.idVendor << " PID:" << descriptor.idProduct << " turning into accessory mode");

  int libusb_ret = 0;

  libusb_ret = libusb_claim_interface(device_handle, 0);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_INFO(
        logger_,
        "libusb_claim_interface failed: " << libusb_error_name(libusb_ret));
    return;
  }

  GetLibusbHandler()->StartControlTransferSequence(
      new AoaInitSequence(device_handle));
}

bool FillUsbDeviceDescription(libusb_device* device,
                              const libusb_device_descriptor& descriptor,
                              libusb_device_handle* device_handle,
                              UsbDeviceDescription* description) {
  int libusb_ret = 0;
  unsigned char buf[128];
  libusb_ret = libusb_get_string_descriptor_ascii(device_handle,
                                                  descriptor.iManufacturer, buf,
                                                  sizeof(buf));
  if (libusb_ret < 0) {
    LOG4CXX_INFO(
        logger_,
        "Failed to get USB device manufacturer: " << libusb_error_name(libusb_ret));
    return false;
  }
  description->manufacturer = std::string(reinterpret_cast<char*>(buf));

  libusb_ret = libusb_get_string_descriptor_ascii(device_handle,
                                                  descriptor.iProduct, buf,
                                                  sizeof(buf));
  if (libusb_ret < 0) {
    LOG4CXX_INFO(
        logger_,
        "Failed to get USB device product name: " << libusb_error_name(libusb_ret));
    return false;
  }
  description->product = std::string(reinterpret_cast<char*>(buf));

  libusb_ret = libusb_get_string_descriptor_ascii(device_handle,
                                                  descriptor.iSerialNumber, buf,
                                                  sizeof(buf));
  if (libusb_ret < 0) {
    LOG4CXX_INFO(
        logger_,
        "Failed to get USB device serial number: " << libusb_error_name(libusb_ret));
    return false;
  }

  description->usb_device = device;
  description->serial_number = std::string(reinterpret_cast<char*>(buf));
  description->vid = descriptor.idVendor;
  description->pid = descriptor.idProduct;
  description->bus_number = libusb_get_bus_number(device);
  description->address = libusb_get_device_address(device);

  return true;
}

void UsbDeviceScanner::GoogleAccessoryFound(
    libusb_device* device, const libusb_device_descriptor& descriptor,
    libusb_device_handle* device_handle) {
  LOG4CXX_INFO(logger_, "Google accessory found");

  UsbDeviceDescription usb_device_description;
  if (FillUsbDeviceDescription(device, descriptor, device_handle,
                               &usb_device_description)) {
    pthread_mutex_lock(&device_descriptions_mutex_);
    device_descriptions_.push_back(usb_device_description);
    pthread_mutex_unlock(&device_descriptions_mutex_);
    LOG4CXX_INFO(
        logger_,
        "Google accessory (bus number " << static_cast<int>(usb_device_description.bus_number) << ", address " << static_cast<int>(usb_device_description.address) << ") identified as: " << usb_device_description.manufacturer << ", " << usb_device_description.product);
  }

  libusb_close(device_handle);
}

TransportAdapter::Error UsbDeviceScanner::Init() {
  return TransportAdapter::OK;
}

TransportAdapter::Error UsbDeviceScanner::Scan() {
  DeviceVector device_vector;
  pthread_mutex_lock(&device_descriptions_mutex_);
  for (DeviceDescriptions::const_iterator it = device_descriptions_.begin();
      it != device_descriptions_.end(); ++it) {
    const std::string device_name = it->manufacturer + it->product;
    std::ostringstream oss;
    oss << it->manufacturer << ":" << it->product << ":" << it->serial_number
        << ":" << static_cast<int>(it->bus_number) << ":"
        << static_cast<int>(it->address);
    const DeviceUID device_uid = oss.str();
    DeviceSptr device(new UsbDevice(it->usb_device, device_name, device_uid));
    device_vector.push_back(device);
  }
  pthread_mutex_unlock(&device_descriptions_mutex_);

  controller_->SearchDeviceDone(device_vector);
  return TransportAdapter::OK;
}

void UsbDeviceScanner::Terminate() {
}

bool UsbDeviceScanner::IsInitialised() const {
  return true;
}

}  // namespace
}  // namespace
