#include "transport_manager/usb/libusb_handler.h"
#include "transport_manager/device_adapter/device_adapter_impl.h"

namespace transport_manager {
namespace device_adapter {

LibusbHandler::LibusbHandler()
    : libusb_context_(0),
      initialised_(false),
      shutdown_requested_(false),
      thread_() {
}

LibusbHandler::~LibusbHandler() {
  pthread_join(thread_, 0);
  LOG4CXX_INFO(logger_, "LibusbHandler thread finished");
  if (libusb_context_) {
    libusb_exit(libusb_context_);
    libusb_context_ = 0;
  }
}

DeviceAdapter::Error LibusbHandler::init() {
  const int libusb_ret = libusb_init(&libusb_context_);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_, "libusb_init failed: " << libusb_ret);
    return DeviceAdapter::FAIL;
  }

  libusb_set_debug(libusb_context_, 3);

  if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
    LOG4CXX_ERROR(logger_, "LIBUSB_CAP_HAS_HOTPLUG not supported");
    return DeviceAdapter::FAIL;
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
    return DeviceAdapter::FAIL;
  }

  return DeviceAdapter::OK;
}

void LibusbHandler::Thread() {
  int completed = 0;
  while (!shutdown_requested_) {
    libusb_handle_events_completed(libusb_context_, &completed);

    for (std::list<UsingLibusbHandler*>::iterator it = using_this_list_.begin();
        it != using_this_list_.end(); ++it) {
      (*it)->OnLibusbHandlerThread();
    }
  }
}

void LibusbHandler::terminate() {
  shutdown_requested_ = true;
}

void* LibusbHandlerThread(void* data) {
  static_cast<LibusbHandler*>(data)->Thread();
  return 0;
}

}  // namespace
}  // namespace

