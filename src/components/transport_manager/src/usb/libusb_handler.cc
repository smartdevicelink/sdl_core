#include <cstring>
#include <cstdlib>

#include "transport_manager/usb/libusb_handler.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

LibusbHandler::LibusbHandler()
    : libusb_context_(NULL),
      arrived_callback_handle_(),
      left_callback_handle_(),
      initialised_(false),
      shutdown_requested_(false),
      thread_() {
}

LibusbHandler::~LibusbHandler() {
  shutdown_requested_ = true;
  if (libusb_context_ != 0) {
    libusb_hotplug_deregister_callback(libusb_context_,
                                       arrived_callback_handle_);
    libusb_hotplug_deregister_callback(libusb_context_, left_callback_handle_);
  }
  pthread_join(thread_, 0);
  LOG4CXX_INFO(logger_, "LibusbHandler thread finished");
  if (libusb_context_) {
    libusb_exit(libusb_context_);
    libusb_context_ = 0;
  }
}

void LibusbHandler::DeviceArrived(libusb_device* device) {
  for (std::list<LibusbListener*>::iterator it = libusb_listeners_.begin();
      it != libusb_listeners_.end(); ++it) {
    (*it)->OnDeviceArrived(device);
  }
}

void LibusbHandler::DeviceLeft(libusb_device* device) {
  for (std::list<LibusbListener*>::iterator it = libusb_listeners_.begin();
      it != libusb_listeners_.end(); ++it) {
    (*it)->OnDeviceLeft(device);
  }
}

void LibusbHandler::StartControlTransferSequence(
    UsbControlTransferSequence* transfer_sequence) {
  usb_control_transfer_sequences_.push_back(transfer_sequence);
  transfer_sequence->Start();
}

void* LibusbHandlerThread(void* data) {
  static_cast<LibusbHandler*>(data)->Thread();
  return 0;
}


int ArrivedCallback(libusb_context* context, libusb_device* device,
                    libusb_hotplug_event event, void* data) {
  LibusbHandler* libusb_handler = static_cast<LibusbHandler*>(data);
  libusb_handler->DeviceArrived(device);
  return 0;
}

int LeftCallback(libusb_context* context, libusb_device* device,
                 libusb_hotplug_event event, void* data) {
  LibusbHandler* libusb_handler = static_cast<LibusbHandler*>(data);
  libusb_handler->DeviceLeft(device);
  return 0;
}

TransportAdapter::Error LibusbHandler::Init() {
  int libusb_ret = libusb_init(&libusb_context_);

  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_, "libusb_init failed: " << libusb_ret);
    return TransportAdapter::FAIL;
  }

  libusb_set_debug(libusb_context_, 3);

  if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
    LOG4CXX_ERROR(logger_, "LIBUSB_CAP_HAS_HOTPLUG not supported");
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
    return TransportAdapter::FAIL;
  }

  const int thread_start_error = pthread_create(&thread_, 0,
                                                &LibusbHandlerThread, this);
  if (0 == thread_start_error) {
    initialised_ = true;
    LOG4CXX_INFO(logger_, "LibusbHandler thread started");
  } else {
    LOG4CXX_ERROR(
        logger_,
        "USB device scanner thread start failed, error code " << thread_start_error);
    return TransportAdapter::FAIL;
  }

  return TransportAdapter::OK;
}

void LibusbHandler::Thread() {
  int completed = 0;
  while (!shutdown_requested_) {
    libusb_handle_events_completed(libusb_context_, &completed);

    for (UsbControlTransferSequences::iterator it =
        usb_control_transfer_sequences_.begin();
        it != usb_control_transfer_sequences_.end();) {
      UsbControlTransferSequence* sequence = *it;
      if (sequence->Finished()) {
        libusb_release_interface(sequence->device_handle(), 0);
        libusb_close(sequence->device_handle());
        delete sequence;
        it = usb_control_transfer_sequences_.erase(it);
      } else {
        ++it;
      }
    }
  }
}

void UsbControlTransferSequence::Start() {
  current_transfer_ = transfers_.begin();
  SubmitTransfer();
}

bool UsbControlTransferSequence::Finished() const {
  return finished_;
}

UsbControlTransferSequence::UsbControlTransferSequence(
    libusb_device_handle* device_handle)
    : device_handle_(device_handle),
      finished_(false) {
}

UsbControlTransferSequence::~UsbControlTransferSequence() {
  for (Transfers::iterator it = transfers_.begin(); it != transfers_.end();
      ++it) {
    delete *it;
  }
}

void UsbControlTransferSequence::AddTransfer(UsbControlTransfer* transfer) {
  transfers_.push_back(transfer);
}

void UsbTransferSequenceCallback(libusb_transfer* transfer) {
  static_cast<UsbControlTransferSequence*>(transfer->user_data)->Callback(
      transfer);
}

void UsbControlTransferSequence::SubmitTransfer() {
  UsbControlTransfer* transfer = *current_transfer_;
  libusb_transfer* libusb_transfer = libusb_alloc_transfer(0);
  if (0 == libusb_transfer) {
    LOG4CXX_ERROR(logger_, "libusb_alloc_transfer failed");
    Finish();
  }

  const libusb_request_type request_type = transfer->RequestType();
  const libusb_endpoint_direction endpoint_direction = transfer->Direction();
  const uint8_t request = transfer->Request();
  const uint16_t value = transfer->Value();
  const uint16_t index = transfer->Index();
  const uint16_t length = transfer->Length();

  unsigned char* buffer = static_cast<unsigned char*>(malloc(
      length + LIBUSB_CONTROL_SETUP_SIZE));
  if (NULL == buffer) {
    LOG4CXX_ERROR(logger_, "buffer allocation failed");
    libusb_free_transfer(libusb_transfer);
    Finish();
  }

  libusb_fill_control_setup(buffer, request_type | endpoint_direction, request,
                            value, index, length);

  if (0 != length && endpoint_direction == LIBUSB_ENDPOINT_OUT) {
    const char* data = static_cast<UsbControlOutTransfer*>(transfer)->Data();
    memcpy(buffer + LIBUSB_CONTROL_SETUP_SIZE, data, length);
  }
  libusb_fill_control_transfer(libusb_transfer, device_handle_, buffer,
                               UsbTransferSequenceCallback, this, 0);
  libusb_transfer->flags = LIBUSB_TRANSFER_FREE_BUFFER;

  const int libusb_ret = libusb_submit_transfer(libusb_transfer);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(
        logger_,
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret));
    libusb_free_transfer(libusb_transfer);
    Finish();
  }
}

void UsbControlTransferSequence::Finish() {
  finished_ = true;
}

libusb_device_handle* UsbControlTransferSequence::device_handle() const {
  return device_handle_;
}

void UsbControlTransferSequence::Callback(libusb_transfer* transfer) {
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    LOG4CXX_INFO(logger_, "USB control transfer completed");
    UsbControlTransfer* current_transfer = *current_transfer_;
    bool submit_next = true;
    if (current_transfer->Direction() == LIBUSB_ENDPOINT_IN) {
      submit_next = static_cast<UsbControlInTransfer*>(current_transfer)
          ->OnCompleted(libusb_control_transfer_get_data(transfer));
    }

    if (submit_next && (++current_transfer_ != transfers_.end())) {
      SubmitTransfer();
    } else {
      Finish();
    }
  } else {
    LOG4CXX_ERROR(logger_, "USB control transfer failed: " << transfer->status);
    Finish();
  }
  libusb_free_transfer(transfer);
}

}  // namespace
}  // namespace

