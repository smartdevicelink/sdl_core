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

#include <sstream>

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/usb/usb_device_scanner.h"
#include "transport_manager/usb/usb_device.h"
#include "transport_manager/usb/common.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

class AoaInitSequence : public UsbControlTransferSequence {
 public:
  AoaInitSequence();
  virtual ~AoaInitSequence() {
  }

 private:
  class AoaGetProtocolRequest;
  class AoaSendIdString;
  class AoaTurnIntoAccessoryMode;
};

void UsbDeviceScanner::OnDeviceArrived(PlatformUsbDevice* device) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (IsAppleDevice(device)) {
    SupportedDeviceFound(device);
  } else {
    if (IsGoogleAccessory(device)) {
      SupportedDeviceFound(device);
    } else {
      TurnIntoAccessoryMode(device);
    }
  }
}

void UsbDeviceScanner::OnDeviceLeft(PlatformUsbDevice* device) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "PlatformUsbDevice " << device);
  bool list_changed = false;
  devices_mutex_.Acquire();
  for (Devices::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    if (device == *it) {
      devices_.erase(it);
      list_changed = true;
      break;
    }
  }
  devices_mutex_.Release();
  if (list_changed) {
    UpdateList();
  }
}

UsbDeviceScanner::UsbDeviceScanner(TransportAdapterController* controller)
    : controller_(controller) {
}

UsbDeviceScanner::~UsbDeviceScanner() {
}

class AoaInitSequence::AoaGetProtocolRequest : public UsbControlInTransfer {
  virtual ~AoaGetProtocolRequest() {
  }
  virtual RequestType Type() const {
    return VENDOR;
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
    LOG4CXX_DEBUG(logger_, "AOA protocol version " << protocol_version);
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
  virtual RequestType Type() const {
    return VENDOR;
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
  virtual RequestType Type() const {
    return VENDOR;
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

static char manufacturer[] = "SDL";
static char model_name[] = "Core";
static char description[] = "SmartDeviceLink Core Component USB";
static char version[] = "1.0";
static char uri[] = "http://www.smartdevicelink.org";
static char serial_num[] = "N000000";

AoaInitSequence::AoaInitSequence()
    : UsbControlTransferSequence() {
  AddTransfer(new AoaGetProtocolRequest);
  AddTransfer(new AoaSendIdString(0, manufacturer, sizeof(manufacturer)));
  AddTransfer(new AoaSendIdString(1, model_name, sizeof(model_name)));
  AddTransfer(new AoaSendIdString(2, description, sizeof(description)));
  AddTransfer(new AoaSendIdString(3, version, sizeof(version)));
  AddTransfer(new AoaSendIdString(4, uri, sizeof(uri)));
  AddTransfer(new AoaSendIdString(5, serial_num, sizeof(serial_num)));
  AddTransfer(new AoaTurnIntoAccessoryMode);
}

void UsbDeviceScanner::TurnIntoAccessoryMode(PlatformUsbDevice* device) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "PlatformUsbDevice: " << device);
  GetUsbHandler()->StartControlTransferSequence(new AoaInitSequence, device);
}

void UsbDeviceScanner::SupportedDeviceFound(PlatformUsbDevice* device) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_TRACE(logger_, "PlatformUsbDevice: " << device);

  devices_mutex_.Acquire();
  devices_.push_back(device);
  devices_mutex_.Release();
  LOG4CXX_DEBUG(
      logger_,
      "USB device (bus number " << static_cast<int>(device->bus_number())
      << ", address " << static_cast<int>(device->address())
      << ") identified as: " << device->GetManufacturer()
      << ", " << device->GetProductName()
      << ", serial: " << device->GetSerialNumber());
  UpdateList();
}

TransportAdapter::Error UsbDeviceScanner::Init() {
  return TransportAdapter::OK;
}

TransportAdapter::Error UsbDeviceScanner::Scan() {
  return TransportAdapter::OK;
}

void UsbDeviceScanner::UpdateList() {
  LOG4CXX_AUTO_TRACE(logger_);
  DeviceVector device_vector;
  devices_mutex_.Acquire();
  for (Devices::const_iterator it = devices_.begin(); it != devices_.end();
      ++it) {
    const std::string device_name = (*it)->GetManufacturer() + " "
        + (*it)->GetProductName();
    std::ostringstream oss;
    oss << (*it)->GetManufacturer() << ":" << (*it)->GetProductName() << ":"
        << (*it)->GetSerialNumber();
    const DeviceUID device_uid = oss.str();
    DeviceSptr device(new UsbDevice(*it, device_name, device_uid));
    device_vector.push_back(device);
  }
  devices_mutex_.Release();

  LOG4CXX_DEBUG(logger_, "USB search done " << device_vector.size());
  controller_->SearchDeviceDone(device_vector);
}

void UsbDeviceScanner::Terminate() {
}

bool UsbDeviceScanner::IsInitialised() const {
  return true;
}

}  // namespace
}  // namespace
