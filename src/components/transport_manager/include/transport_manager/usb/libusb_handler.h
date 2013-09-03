/**
 * \file libusb_handler.h
 * \brief LibusbHandler class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_HANDLER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_HANDLER_H_

#include <pthread.h>
#include <libusb-1.0/libusb.h>

#include "transport_manager/transport_adapter/transport_adapter.h"

namespace transport_manager {

namespace transport_adapter {

class LibusbListener;

class UsbControlTransferSequence {
 public:
  void Start();
  bool Finished() const;
  libusb_device_handle* device_handle() const;
  ~UsbControlTransferSequence();

 protected:
  class UsbControlTransfer;
  class UsbControlInTransfer;
  class UsbControlOutTransfer;

  UsbControlTransferSequence(libusb_device_handle* device_handle);
  void AddTransfer(UsbControlTransfer* transfer);

 private:
  friend void UsbTransferSequenceCallback(libusb_transfer *transfer);

  void Finish();
  void SubmitTransfer();
  void Callback(libusb_transfer* transfer);

  libusb_device_handle* device_handle_;
  typedef std::list<UsbControlTransfer*> Transfers;
  Transfers transfers_;
  Transfers::iterator current_transfer_;
  bool finished_;
};

class UsbControlTransferSequence::UsbControlTransfer {
 public:
  virtual ~UsbControlTransfer() {
  }
  virtual libusb_endpoint_direction Direction() const = 0;
  virtual libusb_request_type RequestType() const = 0;
  virtual uint8_t Request() const = 0;
  virtual uint16_t Value() const = 0;
  virtual uint16_t Index() const = 0;
  virtual uint16_t Length() const = 0;
};

class UsbControlTransferSequence::UsbControlInTransfer :
    public UsbControlTransfer {
 public:
  virtual ~UsbControlInTransfer() {
  }
  virtual libusb_endpoint_direction Direction() const {
    return LIBUSB_ENDPOINT_IN;
  }
  virtual bool OnCompleted(unsigned char* data) const = 0;
};

class UsbControlTransferSequence::UsbControlOutTransfer :
    public UsbControlTransfer {
 public:
  virtual ~UsbControlOutTransfer() {
  }
  virtual libusb_endpoint_direction Direction() const {
    return LIBUSB_ENDPOINT_OUT;
  }
  virtual const char* Data() const = 0;
};

class LibusbHandler {
 public:
  LibusbHandler();
  ~LibusbHandler();
  libusb_context* GetLibusbContext() const {
    return libusb_context_;
  }
  void StartControlTransferSequence(
      UsbControlTransferSequence* transfer_sequence);
  TransportAdapter::Error Init();
 private:
  void DeviceArrived(libusb_device* device);
  void DeviceLeft(libusb_device* device);
  void Thread();
  libusb_context* libusb_context_;
  libusb_hotplug_callback_handle arrived_callback_handle_;
  libusb_hotplug_callback_handle left_callback_handle_;
  bool initialised_;
  bool shutdown_requested_;
  pthread_t thread_;

  typedef std::list<UsbControlTransferSequence*> UsbControlTransferSequences;
  UsbControlTransferSequences usb_control_transfer_sequences_;

  friend class LibusbListener;
  std::list<LibusbListener*> libusb_listeners_;

  friend void* LibusbHandlerThread(void* data);
  friend int ArrivedCallback(libusb_context *context, libusb_device *device,
                             libusb_hotplug_event event, void *data);
  friend int LeftCallback(libusb_context *context, libusb_device *device,
                          libusb_hotplug_event event, void *data);
};

typedef utils::SharedPtr<LibusbHandler> LibusbHandlerSptr;

class LibusbListener {
 public:
  virtual ~LibusbListener() {
  }

  LibusbHandlerSptr GetLibusbHandler() {
    return libusb_handler_;
  }

  void SetLibusbHandler(LibusbHandlerSptr libusb_handler) {
    libusb_handler_ = libusb_handler;
    libusb_handler_->libusb_listeners_.push_back(this);
  }

  virtual void OnDeviceArrived(libusb_device* device) = 0;
  virtual void OnDeviceLeft(libusb_device* device) = 0;
 private:
  LibusbHandlerSptr libusb_handler_;
};

}  // namespace
}  // namespace

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_HANDLER
