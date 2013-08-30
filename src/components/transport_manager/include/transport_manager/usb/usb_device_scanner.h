/**
 * \file usb_device_scanner.h
 * \brief UsbDeviceScanner class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_DEVICE_SCANNER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_DEVICE_SCANNER

#include <list>

#include <pthread.h>
#include <libusb-1.0/libusb.h>

#include "transport_manager/device_adapter/device_scanner.h"
#include "transport_manager/device_adapter/device_adapter.h"

namespace transport_manager {

namespace device_adapter {

class AoaInitSequence;

struct UsbDeviceDescription {
  libusb_device* usb_device;
  uint16_t vid;
  uint16_t pid;
  uint8_t bus_number;
  uint8_t address;
  std::string manufacturer;
  std::string product;
  std::string serial_number;
};

class UsbDeviceScanner : public DeviceScanner, public UsingLibusbHandler {
 public:
  UsbDeviceScanner(class DeviceAdapterController* controller);
  virtual ~UsbDeviceScanner();
 protected:
  virtual DeviceAdapter::Error init();
  virtual DeviceAdapter::Error scan();
  virtual void terminate();
  virtual bool isInitialised() const;
  virtual void OnLibusbHandlerThread();
 private:
  void TurnIntoAccessoryMode(const libusb_device_descriptor& descriptor,
                             libusb_device_handle* device_handle);
  void GoogleAccessoryFound(libusb_device* device,
                            const libusb_device_descriptor& descriptor,
                            libusb_device_handle* device_handle);

  void DeviceArrived(libusb_device* device);
  void DeviceLeft(libusb_device* device);

  DeviceAdapterController* controller_;
  LibusbHandlerSptr libusb_handler_;
  libusb_hotplug_callback_handle arrived_callback_handle_;
  libusb_hotplug_callback_handle left_callback_handle_;

  typedef std::list<AoaInitSequence*> AoaInitSequences;
  AoaInitSequences aoa_init_sequences_;

  typedef std::list<UsbDeviceDescription> DeviceDescriptions;
  DeviceDescriptions device_descriptions_;
  pthread_mutex_t device_descriptions_mutex_;

  friend int ArrivedCallback(libusb_context *context, libusb_device *device,
                             libusb_hotplug_event event, void *data);
  friend int LeftCallback(libusb_context *context, libusb_device *device,
                          libusb_hotplug_event event, void *data);
};

}  // namespace
}  // namespace

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_DEVICE_SCANNER
