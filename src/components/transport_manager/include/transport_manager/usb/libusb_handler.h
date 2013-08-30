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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_HANDLER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_HANDLER

#include <pthread.h>
#include <libusb-1.0/libusb.h>

namespace transport_manager {

namespace device_adapter {

class LibusbHandler {
 public:
  LibusbHandler();
  ~LibusbHandler();
  libusb_context* GetLibusbContext() const {
    return libusb_context_;
  }
 protected:
  DeviceAdapter::Error init();
  void terminate();
 private:
  void Thread();
  libusb_context* libusb_context_;
  bool initialised_;
  bool shutdown_requested_;
  pthread_t thread_;
  friend class UsingLibusbHandler;
  std::list<UsingLibusbHandler*> using_this_list_;
  friend void* LibusbHandlerThread(void* data);
};

typedef utils::SharedPtr<LibusbHandler> LibusbHandlerSptr;

class UsingLibusbHandler {
 public:
  virtual ~UsingLibusbHandler() {
  }

  LibusbHandlerSptr GetLibusbHandler() {
    return libusb_handler_;
  }

  void SetLibusbHandler(LibusbHandlerSptr libusb_handler) {
    libusb_handler_ = libusb_handler;
    libusb_handler_->using_this_list_.push_back(this);
  }

  virtual void OnLibusbHandlerThread() = 0;
 private:
  LibusbHandlerSptr libusb_handler_;
};

}  // namespace
}  // namespace

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_LIBUSB_HANDLER
