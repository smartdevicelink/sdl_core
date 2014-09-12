/**
 * \file usb_handler.cc
 * \brief UsbHandler class source file.
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

#include <cstring>
#include <cstdlib>

#include "transport_manager/usb/common.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

class UsbHandler::ControlTransferSequenceState {
 public:
  ControlTransferSequenceState(class UsbHandler* usb_handler,
                               UsbControlTransferSequence* sequence,
                               PlatformUsbDevice* device);
  ~ControlTransferSequenceState();
  void Finish();
  bool Finished() const {
    return finished_;
  }
  UsbControlTransfer* CurrentTransfer();
  UsbControlTransfer* Next();
  UsbHandler* usb_handler() const {
    return usb_handler_;
  }
  PlatformUsbDevice* device() const {
    return device_;
  }

 private:
  UsbHandler* usb_handler_;
  PlatformUsbDevice* device_;
  bool finished_;
  UsbControlTransferSequence* sequence_;
  UsbControlTransferSequence::Transfers::const_iterator current_transfer_;
};

UsbHandler::UsbHandler()
  : shutdown_requested_(false),
    thread_(),
    usb_device_listeners_(),
    devices_(),
    transfer_sequences_(),
    device_handles_to_close_(),
    libusb_context_(NULL),
    arrived_callback_handle_(),
    left_callback_handle_() {}

UsbHandler::~UsbHandler() {
  shutdown_requested_ = true;
  if (libusb_context_ != 0) {
    libusb_hotplug_deregister_callback(libusb_context_,
                                       arrived_callback_handle_);
    libusb_hotplug_deregister_callback(libusb_context_, left_callback_handle_);
  }
  pthread_join(thread_, 0);
  LOG4CXX_INFO(logger_, "UsbHandler thread finished");
  if (libusb_context_) {
    libusb_exit(libusb_context_);
    libusb_context_ = 0;
  }
}

void UsbHandler::DeviceArrived(libusb_device* device_libusb) {
  LOG4CXX_TRACE(logger_, "enter. libusb_device* " << device_libusb);
  const uint8_t bus_number = libusb_get_bus_number(device_libusb);
  const uint8_t device_address = libusb_get_device_address(device_libusb);
  libusb_device_descriptor descriptor;
  int libusb_ret = libusb_get_device_descriptor(device_libusb, &descriptor);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_, "libusb_get_device_descriptor failed: " << libusb_ret);
    LOG4CXX_TRACE(logger_, "exit. Condition: LIBUSB_SUCCESS != libusb_ret");
    return;
  }

  libusb_device_handle* device_handle_libusb;
  libusb_ret = libusb_open(device_libusb, &device_handle_libusb);
  if (libusb_ret != LIBUSB_SUCCESS) {
    LOG4CXX_ERROR(logger_, "libusb_open failed: " << libusb_error_name(libusb_ret));
    LOG4CXX_TRACE(logger_, "exit. Condition: libusb_ret != LIBUSB_SUCCESS");
    return;
  }

  int configuration;
  libusb_ret = libusb_get_configuration(device_handle_libusb, &configuration);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_INFO(logger_, "libusb_get_configuration failed: " << libusb_error_name(
                   libusb_ret));
    LOG4CXX_TRACE(logger_, "exit. Condition: LIBUSB_SUCCESS != libusb_ret");
    return;
  }

  if (configuration != kUsbConfiguration) {
    libusb_ret = libusb_set_configuration(device_handle_libusb, kUsbConfiguration);
    if (LIBUSB_SUCCESS != libusb_ret) {
      LOG4CXX_INFO(logger_, "libusb_set_configuration failed: " << libusb_error_name(
                     libusb_ret));
      LOG4CXX_TRACE(logger_, "exit. Condition: LIBUSB_SUCCESS != libusb_ret");
      return;
    }
  }

  libusb_ret = libusb_claim_interface(device_handle_libusb, 0);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_INFO(logger_, "libusb_claim_interface failed: " << libusb_error_name(libusb_ret));
    CloseDeviceHandle(device_handle_libusb);
    LOG4CXX_TRACE(logger_, "exit. Condition: LIBUSB_SUCCESS != libusb_ret");
    return;
  }

  PlatformUsbDevice* device(new PlatformUsbDevice(bus_number, device_address,
                            descriptor, device_libusb,
                            device_handle_libusb));
  devices_.push_back(device);

  for (std::list<UsbDeviceListener*>::iterator it =
         usb_device_listeners_.begin();
       it != usb_device_listeners_.end(); ++it) {
    (*it)->OnDeviceArrived(device);
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void UsbHandler::DeviceLeft(libusb_device* device_libusb) {
  LOG4CXX_TRACE(logger_, "enter. libusb_device* " << device_libusb);
  PlatformUsbDevice* device = NULL;
  for (Devices::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    if ((*it)->GetLibusbDevice() == device_libusb) {
      device = *it;
      break;
    }
  }
  if (NULL == device) {
    LOG4CXX_TRACE(logger_, "enter. Condition: NULL == device");
    return;
  }

  for (std::list<UsbDeviceListener*>::iterator it =
         usb_device_listeners_.begin();
       it != usb_device_listeners_.end(); ++it) {
    (*it)->OnDeviceLeft(device);
  }

  for (Devices::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    if ((*it)->GetLibusbDevice() == device_libusb) {
      devices_.erase(it);
      break;
    }
  }

  if (device->GetLibusbHandle()) {
    libusb_release_interface(device->GetLibusbHandle(), 0);
    CloseDeviceHandle(device->GetLibusbHandle());
  }
  delete device;
  LOG4CXX_TRACE(logger_, "exit");
}

void UsbHandler::StartControlTransferSequence(
  UsbControlTransferSequence* sequence, PlatformUsbDevice* device) {
  LOG4CXX_TRACE(logger_, "enter. UsbControlTransferSequence* " << sequence <<
                "PlatformUsbDevice* " << device);
  TransferSequences::iterator it = transfer_sequences_.insert(
                                     transfer_sequences_.end(),
                                     new ControlTransferSequenceState(this, sequence, device));
  SubmitControlTransfer(*it);
  LOG4CXX_TRACE(logger_, "exit");
}

void UsbHandler::CloseDeviceHandle(libusb_device_handle* device_handle) {
  device_handles_to_close_.push_back(device_handle);
}

void* UsbHandlerThread(void* data) {
  static_cast<UsbHandler*>(data)->Thread();
  return 0;
}

int ArrivedCallback(libusb_context* context, libusb_device* device,
                    libusb_hotplug_event event, void* data) {
  LOG4CXX_TRACE(logger_, "enter. libusb device arrived (bus number "
                << static_cast<int>(libusb_get_bus_number(device))
                << ", device address "
                << static_cast<int>(
                  libusb_get_device_address(device)) << ")");
  UsbHandler* usb_handler = static_cast<UsbHandler*>(data);
  usb_handler->DeviceArrived(device);
  LOG4CXX_TRACE(logger_, "exit with 0");
  return 0;
}

int LeftCallback(libusb_context* context, libusb_device* device,
                 libusb_hotplug_event event, void* data) {
  LOG4CXX_TRACE(logger_, "enter libusb device left (bus number "
                << static_cast<int>(libusb_get_bus_number(device))
                << ", device address "
                << static_cast<int>(
                  libusb_get_device_address(device)) << ")");
  UsbHandler* usb_handler = static_cast<UsbHandler*>(data);
  usb_handler->DeviceLeft(device);
  LOG4CXX_TRACE(logger_, "exit with 0");
  return 0;
}

TransportAdapter::Error UsbHandler::Init() {
  LOG4CXX_TRACE(logger_, "enter");
  int libusb_ret = libusb_init(&libusb_context_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_, "libusb_init failed: " << libusb_ret);
    LOG4CXX_TRACE(logger_,
                  "exit with TransportAdapter::FAIL. Condition: LIBUSB_SUCCESS != libusb_ret");
    return TransportAdapter::FAIL;
  }

  if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
    LOG4CXX_ERROR(logger_, "LIBUSB_CAP_HAS_HOTPLUG not supported");
    LOG4CXX_TRACE(logger_,
                  "exit with TransportAdapter::FAIL. Condition: !libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)");
    return TransportAdapter::FAIL;
  }

  libusb_ret = libusb_hotplug_register_callback(
                 libusb_context_, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,
                 LIBUSB_HOTPLUG_ENUMERATE, LIBUSB_HOTPLUG_MATCH_ANY,
                 LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, ArrivedCallback, this,
                 &arrived_callback_handle_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_,
                  "libusb_hotplug_register_callback failed: " << libusb_ret);
    LOG4CXX_TRACE(logger_,
                  "exit with TransportAdapter::FAIL. Condition: LIBUSB_SUCCESS != libusb_ret");
    return TransportAdapter::FAIL;
  }

  libusb_ret = libusb_hotplug_register_callback(
                 libusb_context_, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
                 static_cast<libusb_hotplug_flag>(0), LIBUSB_HOTPLUG_MATCH_ANY,
                 LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, LeftCallback, this,
                 &left_callback_handle_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_,
                  "libusb_hotplug_register_callback failed: " << libusb_ret);
    LOG4CXX_TRACE(logger_,
                  "exit with TransportAdapter::FAIL. Condition: LIBUSB_SUCCESS != libusb_ret");
    return TransportAdapter::FAIL;
  }

  const int thread_start_error =
    pthread_create(&thread_, 0, &UsbHandlerThread, this);
  if (0 != thread_start_error) {
    LOG4CXX_ERROR(logger_, "USB device scanner thread start failed, error code "
                  << thread_start_error);
    LOG4CXX_TRACE(logger_,
                  "exit with TransportAdapter::FAIL. Condition: 0 !== thread_start_error");
    return TransportAdapter::FAIL;
  }
  LOG4CXX_INFO(logger_, "UsbHandler thread started");
  pthread_setname_np(thread_, "UsbHandler" );
  LOG4CXX_TRACE(logger_,
                "exit with TransportAdapter::OK. Condition: 0 == thread_start_error");
  return TransportAdapter::OK;
}

void UsbHandler::Thread() {
  LOG4CXX_TRACE(logger_, "enter");
  int completed = 0;
  while (!shutdown_requested_) {
    libusb_handle_events_completed(libusb_context_, &completed);

    for (TransferSequences::iterator it = transfer_sequences_.begin();
         it != transfer_sequences_.end();) {
      ControlTransferSequenceState* sequence_state = *it;
      if (sequence_state->Finished()) {
        delete *it;
        it = transfer_sequences_.erase(it);
      } else {
        ++it;
      }
    }

    for (std::list<libusb_device_handle*>::iterator it =
           device_handles_to_close_.begin();
         it != device_handles_to_close_.end();
         it = device_handles_to_close_.erase(it)) {
      libusb_close(*it);
    }
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void UsbTransferSequenceCallback(libusb_transfer* transfer) {
  LOG4CXX_TRACE(logger_, "enter. libusb_transfer* " << transfer);
  UsbHandler::ControlTransferSequenceState* sequence_state =
    static_cast<UsbHandler::ControlTransferSequenceState*>(transfer->user_data);
  sequence_state->usb_handler()->ControlTransferCallback(transfer);
  LOG4CXX_TRACE(logger_, "exit");
}

void UsbHandler::SubmitControlTransfer(
  ControlTransferSequenceState* sequence_state) {
  LOG4CXX_TRACE(logger_, "enter. ControlTransferSequenceState* " << sequence_state);
  UsbControlTransfer* transfer = sequence_state->CurrentTransfer();
  if (NULL == transfer) {
    LOG4CXX_TRACE(logger_, "exit. Condition: NULL == transfer");
    return;
  }

  libusb_transfer* libusb_transfer = libusb_alloc_transfer(0);
  if (0 == libusb_transfer) {
    LOG4CXX_ERROR(logger_, "libusb_alloc_transfer failed");
    sequence_state->Finish();
    LOG4CXX_TRACE(logger_, "exit. Condition: 0 == libusb_transfer");
    return;
  }

  assert(transfer->Type() == UsbControlTransfer::VENDOR);
  const libusb_request_type request_type = LIBUSB_REQUEST_TYPE_VENDOR;

  libusb_endpoint_direction endpoint_direction;
  if (transfer->Direction() == UsbControlTransfer::IN) {
    endpoint_direction = LIBUSB_ENDPOINT_IN;
  } else if (transfer->Direction() == UsbControlTransfer::OUT) {
    endpoint_direction = LIBUSB_ENDPOINT_OUT;
  } else {
    assert(0);
  }
  const uint8_t request = transfer->Request();
  const uint16_t value = transfer->Value();
  const uint16_t index = transfer->Index();
  const uint16_t length = transfer->Length();

  unsigned char* buffer =
    static_cast<unsigned char*>(malloc(length + LIBUSB_CONTROL_SETUP_SIZE));
  if (NULL == buffer) {
    LOG4CXX_ERROR(logger_, "buffer allocation failed");
    libusb_free_transfer(libusb_transfer);
    sequence_state->Finish();
    LOG4CXX_TRACE(logger_, "exit. Condition: NULL == buffer");
    return;
  }

  libusb_fill_control_setup(buffer, request_type | endpoint_direction, request,
                            value, index, length);

  if (0 != length && endpoint_direction == LIBUSB_ENDPOINT_OUT) {
    const char* data = static_cast<UsbControlOutTransfer*>(transfer)->Data();
    memcpy(buffer + LIBUSB_CONTROL_SETUP_SIZE, data, length);
  }
  libusb_fill_control_transfer(
    libusb_transfer, sequence_state->device()->GetLibusbHandle(), buffer,
    UsbTransferSequenceCallback, sequence_state, 0);
  libusb_transfer->flags = LIBUSB_TRANSFER_FREE_BUFFER;

  const int libusb_ret = libusb_submit_transfer(libusb_transfer);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_, "libusb_submit_transfer failed: "
                  << libusb_error_name(libusb_ret));
    libusb_free_transfer(libusb_transfer);
    sequence_state->Finish();
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void UsbHandler::ControlTransferCallback(libusb_transfer* transfer) {
  LOG4CXX_TRACE(logger_, "enter. libusb_transfer* " << transfer);
  ControlTransferSequenceState* sequence_state =
    static_cast<ControlTransferSequenceState*>(transfer->user_data);
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    LOG4CXX_INFO(logger_, "USB control transfer completed");
    UsbControlTransfer* current_transfer = sequence_state->CurrentTransfer();
    bool submit_next = true;
    if (current_transfer &&
        current_transfer->Direction() == UsbControlTransfer::IN) {
      submit_next =
        static_cast<UsbControlInTransfer*>(current_transfer)
        ->OnCompleted(libusb_control_transfer_get_data(transfer));
    }

    sequence_state->Next();
    if (submit_next && !sequence_state->Finished()) {
      SubmitControlTransfer(sequence_state);
    } else {
      sequence_state->Finish();
    }
  } else {
    LOG4CXX_ERROR(logger_, "USB control transfer failed: " << transfer->status);
    sequence_state->Finish();
  }
  libusb_free_transfer(transfer);
  LOG4CXX_TRACE(logger_, "exit");
}

UsbHandler::ControlTransferSequenceState::ControlTransferSequenceState(
  UsbHandler* usb_handler, UsbControlTransferSequence* sequence,
  PlatformUsbDevice* device)
  : usb_handler_(usb_handler),
    device_(device),
    finished_(false),
    sequence_(sequence),
    current_transfer_(sequence->transfers().begin()) {}

UsbHandler::ControlTransferSequenceState::~ControlTransferSequenceState() {
  delete sequence_;
}

UsbControlTransfer* UsbHandler::ControlTransferSequenceState::Next() {
  LOG4CXX_TRACE(logger_, "enter");
  if (finished_) {
    LOG4CXX_TRACE(logger_, "exit with NULL. Condition: finished_");
    return NULL;
  }
  if (++current_transfer_ == sequence_->transfers().end()) {
    Finish();
    LOG4CXX_TRACE(logger_,
                  "exit with NULL. Condition: ++current_transfer_ == sequence_->transfers().end()");
    return NULL;
  } else {
    LOG4CXX_TRACE(logger_, "exit with UsbControlTransfer* " << *current_transfer_ <<
                  ".Condition: ++current_transfer_ !== sequence_->transfers().end()");
    return *current_transfer_;
  }
}

UsbControlTransfer* UsbHandler::ControlTransferSequenceState::CurrentTransfer() {
  return finished_ ? NULL : *current_transfer_;
}

void UsbHandler::ControlTransferSequenceState::Finish() {
  finished_ = true;
}

}  // namespace
}  // namespace
