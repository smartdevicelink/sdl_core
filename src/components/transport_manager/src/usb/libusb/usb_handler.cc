/*
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

#include <cstdlib>
#include <cstring>

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/usb/common.h"

#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/threads/thread.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

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
    : shutdown_requested_(false)
    , thread_(nullptr)
    , usb_device_listeners_()
    , devices_()
    , transfer_sequences_()
    , device_handles_to_close_()
    , libusb_context_(nullptr)
    , arrived_callback_handle_()
    , left_callback_handle_() {
  thread_ = threads::CreateThread("UsbHandler", new UsbHandlerDelegate(this));
}

UsbHandler::~UsbHandler() {
  SDL_LOG_AUTO_TRACE();

  // Notify Thread to stop on next iteration...
  RequestStopThread();

  // ... and unregister hotplug callbacks in order to wake
  // libusb_handle_events()
  DeregisterHotplugCallbacks();

  // Now it is safe to join the Thread and free up resources
  JoinAndDeleteThread();
  InvokeLibusbExit();
}

void UsbHandler::RequestStopThread() {
  SDL_LOG_AUTO_TRACE();
  shutdown_requested_.store(true);
}

void UsbHandler::DeregisterHotplugCallbacks() {
  SDL_LOG_AUTO_TRACE();

  if (libusb_context_) {
    // The libusb_hotplug_deregister_callback() wakes up blocking call of
    // libusb_handle_events_completed() in the Thread() method of delegate
    libusb_hotplug_deregister_callback(libusb_context_,
                                       arrived_callback_handle_);
    libusb_hotplug_deregister_callback(libusb_context_, left_callback_handle_);
  }
}

void UsbHandler::JoinAndDeleteThread() {
  SDL_LOG_AUTO_TRACE();

  // Let thread finish on its own
  thread_->Stop(threads::Thread::kThreadSoftStop);
  delete thread_->GetDelegate();
  threads::DeleteThread(thread_);
  SDL_LOG_INFO("UsbHandler thread finished");
}

void UsbHandler::InvokeLibusbExit() {
  SDL_LOG_AUTO_TRACE();

  if (libusb_context_) {
    libusb_exit(libusb_context_);
    libusb_context_ = nullptr;
  }
}

void UsbHandler::DeviceArrived(libusb_device* device_libusb) {
  SDL_LOG_TRACE("enter. libusb_device* " << device_libusb);
  const uint8_t bus_number = libusb_get_bus_number(device_libusb);
  const uint8_t device_address = libusb_get_device_address(device_libusb);
  libusb_device_descriptor descriptor;
  int libusb_ret = libusb_get_device_descriptor(device_libusb, &descriptor);
  if (LIBUSB_SUCCESS != libusb_ret) {
    SDL_LOG_ERROR("libusb_get_device_descriptor failed: " << libusb_ret);
    SDL_LOG_TRACE("exit. Condition: LIBUSB_SUCCESS != libusb_ret");
    return;
  }

  libusb_device_handle* device_handle_libusb;
  libusb_ret = libusb_open(device_libusb, &device_handle_libusb);
  if (libusb_ret != LIBUSB_SUCCESS) {
    SDL_LOG_ERROR("libusb_open failed: " << libusb_error_name(libusb_ret));
    SDL_LOG_TRACE("exit. Condition: libusb_ret != LIBUSB_SUCCESS");
    return;
  }

  int configuration;
  libusb_ret = libusb_get_configuration(device_handle_libusb, &configuration);
  if (LIBUSB_SUCCESS != libusb_ret) {
    SDL_LOG_INFO(
        "libusb_get_configuration failed: " << libusb_error_name(libusb_ret));
    SDL_LOG_TRACE("exit. Condition: LIBUSB_SUCCESS != libusb_ret");
    return;
  }

  if (configuration != kUsbConfiguration) {
    libusb_ret =
        libusb_set_configuration(device_handle_libusb, kUsbConfiguration);
    if (LIBUSB_SUCCESS != libusb_ret) {
      SDL_LOG_INFO(
          "libusb_set_configuration failed: " << libusb_error_name(libusb_ret));
      SDL_LOG_TRACE("exit. Condition: LIBUSB_SUCCESS != libusb_ret");
      return;
    }
  }

  libusb_ret = libusb_claim_interface(device_handle_libusb, 0);
  if (LIBUSB_SUCCESS != libusb_ret) {
    SDL_LOG_INFO(
        "libusb_claim_interface failed: " << libusb_error_name(libusb_ret));
    CloseDeviceHandle(device_handle_libusb);
    SDL_LOG_TRACE("exit. Condition: LIBUSB_SUCCESS != libusb_ret");
    return;
  }

  PlatformUsbDevice* device(new PlatformUsbDevice(bus_number,
                                                  device_address,
                                                  descriptor,
                                                  device_libusb,
                                                  device_handle_libusb));
  devices_.push_back(device);

  for (std::list<UsbDeviceListener*>::iterator it =
           usb_device_listeners_.begin();
       it != usb_device_listeners_.end();
       ++it) {
    (*it)->OnDeviceArrived(device);
  }
  SDL_LOG_TRACE("exit");
}

void UsbHandler::DeviceLeft(libusb_device* device_libusb) {
  SDL_LOG_TRACE("enter. libusb_device* " << device_libusb);
  PlatformUsbDevice* device = NULL;
  for (Devices::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    if ((*it)->GetLibusbDevice() == device_libusb) {
      device = *it;
      break;
    }
  }
  if (NULL == device) {
    SDL_LOG_TRACE("enter. Condition: NULL == device");
    return;
  }

  for (std::list<UsbDeviceListener*>::iterator it =
           usb_device_listeners_.begin();
       it != usb_device_listeners_.end();
       ++it) {
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
  SDL_LOG_TRACE("exit");
}

void UsbHandler::StartControlTransferSequence(
    UsbControlTransferSequence* sequence, PlatformUsbDevice* device) {
  SDL_LOG_TRACE("enter. UsbControlTransferSequence* "
                << sequence << "PlatformUsbDevice* " << device);
  TransferSequences::iterator it = transfer_sequences_.insert(
      transfer_sequences_.end(),
      new ControlTransferSequenceState(this, sequence, device));
  SubmitControlTransfer(*it);
  SDL_LOG_TRACE("exit");
}

void UsbHandler::CloseDeviceHandle(libusb_device_handle* device_handle) {
  device_handles_to_close_.push_back(device_handle);
}

int ArrivedCallback(libusb_context* context,
                    libusb_device* device,
                    libusb_hotplug_event event,
                    void* data) {
  SDL_LOG_TRACE("enter. libusb device arrived (bus number "
                << static_cast<int>(libusb_get_bus_number(device))
                << ", device address "
                << static_cast<int>(libusb_get_device_address(device)) << ")");
  UsbHandler* usb_handler = static_cast<UsbHandler*>(data);
  usb_handler->DeviceArrived(device);
  SDL_LOG_TRACE("exit with 0");
  return 0;
}

int LeftCallback(libusb_context* context,
                 libusb_device* device,
                 libusb_hotplug_event event,
                 void* data) {
  SDL_LOG_TRACE("enter libusb device left (bus number "
                << static_cast<int>(libusb_get_bus_number(device))
                << ", device address "
                << static_cast<int>(libusb_get_device_address(device)) << ")");
  UsbHandler* usb_handler = static_cast<UsbHandler*>(data);
  usb_handler->DeviceLeft(device);
  SDL_LOG_TRACE("exit with 0");
  return 0;
}

TransportAdapter::Error UsbHandler::Init() {
  SDL_LOG_TRACE("enter");
  int libusb_ret = libusb_init(&libusb_context_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    SDL_LOG_ERROR("libusb_init failed: " << libusb_ret);
    SDL_LOG_TRACE(
        "exit with TransportAdapter::FAIL. Condition: LIBUSB_SUCCESS "
        "!= libusb_ret");
    return TransportAdapter::FAIL;
  }

  if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
    SDL_LOG_ERROR("LIBUSB_CAP_HAS_HOTPLUG not supported");
    SDL_LOG_TRACE(
        "exit with TransportAdapter::FAIL. Condition: "
        "!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)");
    return TransportAdapter::FAIL;
  }

  libusb_ret =
      libusb_hotplug_register_callback(libusb_context_,
                                       LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,
                                       LIBUSB_HOTPLUG_ENUMERATE,
                                       LIBUSB_HOTPLUG_MATCH_ANY,
                                       LIBUSB_HOTPLUG_MATCH_ANY,
                                       LIBUSB_HOTPLUG_MATCH_ANY,
                                       ArrivedCallback,
                                       this,
                                       &arrived_callback_handle_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    SDL_LOG_ERROR("libusb_hotplug_register_callback failed: " << libusb_ret);
    SDL_LOG_TRACE(
        "exit with TransportAdapter::FAIL. Condition: LIBUSB_SUCCESS "
        "!= libusb_ret");
    return TransportAdapter::FAIL;
  }

  libusb_ret =
      libusb_hotplug_register_callback(libusb_context_,
                                       LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
                                       static_cast<libusb_hotplug_flag>(0),
                                       LIBUSB_HOTPLUG_MATCH_ANY,
                                       LIBUSB_HOTPLUG_MATCH_ANY,
                                       LIBUSB_HOTPLUG_MATCH_ANY,
                                       LeftCallback,
                                       this,
                                       &left_callback_handle_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    SDL_LOG_ERROR("libusb_hotplug_register_callback failed: " << libusb_ret);
    SDL_LOG_TRACE(
        "exit with TransportAdapter::FAIL. Condition: LIBUSB_SUCCESS "
        "!= libusb_ret");
    return TransportAdapter::FAIL;
  }

  if (!thread_->Start()) {
    SDL_LOG_ERROR("USB device scanner thread start failed, error code");
    SDL_LOG_TRACE("exit with TransportAdapter::FAIL.");
    return TransportAdapter::FAIL;
  }
  return TransportAdapter::OK;
}

void UsbHandler::Thread() {
  SDL_LOG_TRACE("enter");
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
  SDL_LOG_TRACE("exit");
}

void UsbTransferSequenceCallback(libusb_transfer* transfer) {
  SDL_LOG_TRACE("enter. libusb_transfer* " << transfer);
  UsbHandler::ControlTransferSequenceState* sequence_state =
      static_cast<UsbHandler::ControlTransferSequenceState*>(
          transfer->user_data);
  sequence_state->usb_handler()->ControlTransferCallback(transfer);
  SDL_LOG_TRACE("exit");
}

void UsbHandler::SubmitControlTransfer(
    ControlTransferSequenceState* sequence_state) {
  SDL_LOG_TRACE("enter. ControlTransferSequenceState* " << sequence_state);
  UsbControlTransfer* transfer = sequence_state->CurrentTransfer();
  if (NULL == transfer) {
    SDL_LOG_TRACE("exit. Condition: NULL == transfer");
    return;
  }

  libusb_transfer* libusb_transfer = libusb_alloc_transfer(0);
  if (0 == libusb_transfer) {
    SDL_LOG_ERROR("libusb_alloc_transfer failed");
    sequence_state->Finish();
    SDL_LOG_TRACE("exit. Condition: 0 == libusb_transfer");
    return;
  }

  assert(transfer->Type() == UsbControlTransfer::VENDOR);
  const libusb_request_type request_type = LIBUSB_REQUEST_TYPE_VENDOR;

  libusb_endpoint_direction endpoint_direction = LIBUSB_ENDPOINT_IN;
  if (transfer->Direction() == UsbControlTransfer::IN) {
    endpoint_direction = LIBUSB_ENDPOINT_IN;
  } else if (transfer->Direction() == UsbControlTransfer::OUT) {
    endpoint_direction = LIBUSB_ENDPOINT_OUT;
  } else {
    NOTREACHED();
  }
  const uint8_t request = transfer->Request();
  const uint16_t value = transfer->Value();
  const uint16_t index = transfer->Index();
  const uint16_t length = transfer->Length();

  unsigned char* buffer =
      static_cast<unsigned char*>(malloc(length + LIBUSB_CONTROL_SETUP_SIZE));
  if (NULL == buffer) {
    SDL_LOG_ERROR("buffer allocation failed");
    libusb_free_transfer(libusb_transfer);
    sequence_state->Finish();
    SDL_LOG_TRACE("exit. Condition: NULL == buffer");
    return;
  }

  libusb_fill_control_setup(
      buffer, request_type | endpoint_direction, request, value, index, length);

  if (0 != length && endpoint_direction == LIBUSB_ENDPOINT_OUT) {
    const char* data = static_cast<UsbControlOutTransfer*>(transfer)->Data();
    memcpy(buffer + LIBUSB_CONTROL_SETUP_SIZE, data, length);
  }
  libusb_fill_control_transfer(libusb_transfer,
                               sequence_state->device()->GetLibusbHandle(),
                               buffer,
                               UsbTransferSequenceCallback,
                               sequence_state,
                               0);
  libusb_transfer->flags = LIBUSB_TRANSFER_FREE_BUFFER;

  const int libusb_ret = libusb_submit_transfer(libusb_transfer);
  if (LIBUSB_SUCCESS != libusb_ret) {
    SDL_LOG_ERROR(
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret));
    libusb_free_transfer(libusb_transfer);
    sequence_state->Finish();
  }
  SDL_LOG_TRACE("exit");
}

void UsbHandler::ControlTransferCallback(libusb_transfer* transfer) {
  SDL_LOG_TRACE("enter. libusb_transfer* " << transfer);
  ControlTransferSequenceState* sequence_state =
      static_cast<ControlTransferSequenceState*>(transfer->user_data);
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    SDL_LOG_INFO("USB control transfer completed");
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
    SDL_LOG_ERROR("USB control transfer failed: " << transfer->status);
    sequence_state->Finish();
  }
  libusb_free_transfer(transfer);
  SDL_LOG_TRACE("exit");
}

UsbHandler::ControlTransferSequenceState::ControlTransferSequenceState(
    UsbHandler* usb_handler,
    UsbControlTransferSequence* sequence,
    PlatformUsbDevice* device)
    : usb_handler_(usb_handler)
    , device_(device)
    , finished_(false)
    , sequence_(sequence)
    , current_transfer_(sequence->transfers().begin()) {}

UsbHandler::ControlTransferSequenceState::~ControlTransferSequenceState() {
  delete sequence_;
}

UsbControlTransfer* UsbHandler::ControlTransferSequenceState::Next() {
  SDL_LOG_TRACE("enter");
  if (finished_) {
    SDL_LOG_TRACE("exit with NULL. Condition: finished_");
    return NULL;
  }
  if (++current_transfer_ == sequence_->transfers().end()) {
    Finish();
    SDL_LOG_TRACE(
        "exit with NULL. Condition: ++current_transfer_ == "
        "sequence_->transfers().end()");
    return NULL;
  } else {
    SDL_LOG_TRACE("exit with UsbControlTransfer* "
                  << *current_transfer_
                  << ".Condition: ++current_transfer_ !== "
                     "sequence_->transfers().end()");
    return *current_transfer_;
  }
}

UsbControlTransfer*
UsbHandler::ControlTransferSequenceState::CurrentTransfer() {
  return finished_ ? NULL : *current_transfer_;
}

void UsbHandler::ControlTransferSequenceState::Finish() {
  finished_ = true;
}

UsbHandler::UsbHandlerDelegate::UsbHandlerDelegate(UsbHandler* handler)
    : handler_(handler) {}

void UsbHandler::UsbHandlerDelegate::threadMain() {
  SDL_LOG_AUTO_TRACE();
  DCHECK(handler_);
  handler_->Thread();
}

void UsbHandler::UsbHandlerDelegate::exitThreadMain() {
  SDL_LOG_AUTO_TRACE();
  // Empty method required in order to avoid force delegate thread
  // finishing by exitThreadMain() of the base class
}

}  // namespace transport_adapter
}  // namespace transport_manager
