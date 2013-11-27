/**
 * \file usb_handler.h
 * \brief libusb USB handler class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_USB_HANDLER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_USB_HANDLER_H_

#include <pthread.h>

#include <libusb/libusb.h>

#include "transport_manager/transport_adapter/transport_adapter.h"
#include "transport_manager/usb/usb_control_transfer.h"

namespace transport_manager {

namespace transport_adapter {

class PlatformUsbDevice {
 public:
  uint8_t bus_number() const { return bus_number_; }
  uint8_t address() const { return address_; }
  uint16_t vendor_id() const { return vendor_id_; }
  uint16_t product_id() const { return product_id_; }
  std::string GetManufacturer() const;
  std::string GetProductName() const;
  std::string GetSerialNumber() const;
  PlatformUsbDevice(uint8_t bus, uint8_t address,
                    const libusb_device_descriptor& device_descriptor,
                    libusb_device* device_libusb,
                    libusb_device_handle* device_handle_libusb);
  libusb_device_handle* GetLibusbHandle() { return libusb_device_handle_; }
  libusb_device* GetLibusbDevice() { return libusb_device_; }

 private:
  std::string GetDescString(uint8_t index) const;

 private:
  uint8_t bus_number_;
  uint8_t address_;
  uint16_t vendor_id_;
  uint16_t product_id_;
  libusb_device_descriptor device_descriptor_;
  libusb_device_handle* libusb_device_handle_;
  libusb_device* libusb_device_;
};

class UsbControlTransferSequenceState {
 public:
  UsbControlTransferSequenceState(class UsbHandler* usb_handler,
                                  UsbControlTransferSequence* sequence,
                                  PlatformUsbDevice* device);
  ~UsbControlTransferSequenceState();
  void Finish();
  bool Finished() const { return finished_; }
  UsbControlTransfer* CurrentTransfer();
  UsbControlTransfer* Next();
  UsbHandler* usb_handler() const { return usb_handler_; }
  PlatformUsbDevice* device() const { return device_; }

 private:
  UsbHandler* usb_handler_;
  PlatformUsbDevice* device_;
  bool finished_;
  UsbControlTransferSequence* sequence_;
  UsbControlTransferSequence::Transfers::const_iterator current_transfer_;
};

class UsbHandler {
 public:
  UsbHandler();
  ~UsbHandler();
  void StartControlTransferSequence(UsbControlTransferSequence* sequence,
                                    PlatformUsbDevice* device);
  TransportAdapter::Error Init();

 public:
  void CloseDeviceHandle(libusb_device_handle* device_handle);

 private:
  void Thread();
  void DeviceArrived(libusb_device* device);
  void DeviceLeft(libusb_device* device);

  void ControlTransferCallback(libusb_transfer* transfer);
  void SubmitControlTransfer(UsbControlTransferSequenceState* sequence_state);
  friend void UsbTransferSequenceCallback(libusb_transfer* transfer);

 private:
  bool shutdown_requested_;
  pthread_t thread_;

  friend class UsbDeviceListener;
  std::list<class UsbDeviceListener*> usb_device_listeners_;

  typedef std::vector<PlatformUsbDevice*> Devices;
  Devices devices_;

  typedef std::list<UsbControlTransferSequenceState*> TransferSequences;
  TransferSequences transfer_sequences_;
  std::list<libusb_device_handle*> device_handles_to_close_;
  libusb_context* libusb_context_;
  libusb_hotplug_callback_handle arrived_callback_handle_;
  libusb_hotplug_callback_handle left_callback_handle_;

  friend void* UsbHandlerThread(void* data);
  friend int ArrivedCallback(libusb_context* context, libusb_device* device,
                             libusb_hotplug_event event, void* data);
  friend int LeftCallback(libusb_context* context, libusb_device* device,
                          libusb_hotplug_event event, void* data);
};

}  // namespace
}  // namespace

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_USB_HANDLER_H_
