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

#include <errno.h>

#include <cstring>
#include <cstdlib>

#include "transport_manager/usb/common.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

namespace {
UsbHandler* usb_handler;
}

UsbHandler::UsbHandler()
    : usb_device_listeners_(),
      devices_(),
      usbd_general_connection_(NULL),
      usbd_aoa_connection_(NULL) {
  usb_handler = this;
}

UsbHandler::~UsbHandler() {
  usb_handler = NULL;
  if (usbd_general_connection_) {
    const int disconnect_rc = usbd_disconnect(usbd_general_connection_);
    if (EOK != disconnect_rc) {
      LOG4CXX_ERROR(logger_, "usbd_disconnect failed, error code "
                                 << disconnect_rc);
    }
  }
  if (usbd_aoa_connection_) {
    const int disconnect_rc = usbd_disconnect(usbd_aoa_connection_);
    if (EOK != disconnect_rc) {
      LOG4CXX_ERROR(logger_, "usbd_disconnect failed, error code "
                                 << disconnect_rc);
    }
  }
}

bool operator==(const usbd_device_instance_t& a,
                const usbd_device_instance_t& b) {
  return a.path == b.path && a.devno == b.devno;
}

void UsbHandler::DeviceArrived(usbd_connection* connection,
                               usbd_device_instance_t* instance) {
  for (Devices::const_iterator it = devices_.begin(); it != devices_.end();
       ++it) {
    if ((*it)->GetDeviceInstance() == *instance) return;
  }
  usbd_device* device_usbd = 0;
  const int attach_rc = usbd_attach(connection, instance, 0, &device_usbd);
  if (EOK != attach_rc) {
    LOG4CXX_ERROR(logger_, "usbd_attach failed: " << attach_rc);
    return;
  }

  usbd_desc_node* node;
  usbd_device_descriptor_t* descriptor =
      usbd_device_descriptor(device_usbd, &node);
  if (NULL == descriptor) {
    LOG4CXX_ERROR(logger_, "usbd_device_descriptor failed");
    return;
  }

  PlatformUsbDevice* device(
      new PlatformUsbDevice(instance, device_usbd, *descriptor));
  devices_.push_back(device);
  for (std::list<UsbDeviceListener*>::iterator it =
           usb_device_listeners_.begin();
       it != usb_device_listeners_.end(); ++it) {
    (*it)->OnDeviceArrived(device);
  }
}

void UsbHandler::DeviceLeft(usbd_device_instance_t* instance) {
  PlatformUsbDevice* device = NULL;
  for (Devices::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    if (((*it)->GetDeviceInstance()) == *instance) {
      device = *it;
      break;
    }
  }
  if (NULL == device) {
    return;
  }

  for (std::list<UsbDeviceListener*>::iterator it =
           usb_device_listeners_.begin();
       it != usb_device_listeners_.end(); ++it) {
    (*it)->OnDeviceLeft(device);
  }

  for (Devices::iterator it = devices_.begin(); it != devices_.end(); ++it) {
    if ((*it)->GetDeviceInstance() == *instance) {
      const int detach_rc = usbd_detach((*it)->GetUsbdDevice());
      if (EOK != detach_rc)
        LOG4CXX_ERROR(logger_, "usbd_detach failed: " << detach_rc);
      devices_.erase(it);
      break;
    }
  }

  delete device;
}

void UsbHandler::StartControlTransferSequence(
    UsbControlTransferSequence* sequence, PlatformUsbDevice* device) {
  usbd_descriptors_t* descriptor = usbd_parse_descriptors(
      device->GetUsbdDevice(), NULL, USB_DESC_DEVICE, 0, NULL);
  if (NULL == descriptor) {
    LOG4CXX_ERROR(logger_, "usbd_parse_descriptors failed");
  }

  usbd_pipe* usb_pipe = 0;
  const int open_pipe_rc =
      usbd_open_pipe(device->GetUsbdDevice(), descriptor, &usb_pipe);
  if (EOK != open_pipe_rc) {
    LOG4CXX_ERROR(logger_, "usbd_open_pipe failed, error " << open_pipe_rc);
    return;
  }

  for (UsbControlTransferSequence::Transfers::const_iterator it =
           sequence->transfers().begin();
       it != sequence->transfers().end(); ++it) {
    UsbControlTransfer* transfer = *it;

    usbd_urb* urb = usbd_alloc_urb(NULL);
    if (NULL == urb) {
      LOG4CXX_ERROR(logger_, "usbd_alloc_urb failed");
      break;
    }

    unsigned char* buf =
        static_cast<unsigned char*>(usbd_alloc(transfer->Length()));
    if (NULL == buf) {
      LOG4CXX_ERROR(logger_, "usbd_alloc failed");
      break;
    }

    uint32_t flags = 0;
    if (transfer->Direction() == UsbControlTransfer::IN) {
      flags = URB_DIR_IN;
    } else if (transfer->Direction() == UsbControlTransfer::OUT) {
      flags = URB_DIR_OUT;
      UsbControlOutTransfer* out_transfer =
          static_cast<UsbControlOutTransfer*>(transfer);
      std::copy(out_transfer->Data(),
                out_transfer->Data() + out_transfer->Length(), buf);
    } else {
      assert(0);
    }

    assert(transfer->Type() == UsbControlTransfer::VENDOR);

    usbd_setup_vendor(urb, flags, transfer->Request(), USB_TYPE_VENDOR,
                      transfer->Value(), transfer->Index(), buf,
                      transfer->Length());
    const int io_rc = usbd_io(urb, usb_pipe, 0, 0, USBD_TIME_DEFAULT);

    bool submit_next = true;
    if (io_rc == EOK) {
      if (transfer->Direction() == UsbControlTransfer::IN) {
        submit_next =
            static_cast<UsbControlInTransfer*>(transfer)->OnCompleted(buf);
      }
    } else {
      LOG4CXX_ERROR(logger_, "usbd_io failed, error " << io_rc);
      submit_next = false;
    }
    usbd_free(buf);
    usbd_free_urb(urb);
    if (!submit_next) break;
  }
  usbd_close_pipe(usb_pipe);

  delete sequence;
}

void ArrivedCallback(usbd_connection* connection,
                     usbd_device_instance_t* instance) {
  if (kAoaVid == instance->ident.vendor) return;
  LOG4CXX_INFO(logger_, "USB device arrived (path "
                            << static_cast<int>(instance->path) << ", devno "
                            << static_cast<int>(instance->devno) << ", config "
                            << static_cast<int>(instance->config) << ", iface "
                            << static_cast<int>(instance->iface) << ")");
  usb_handler->DeviceArrived(connection, instance);
}

void ArrivedAoaCallback(usbd_connection* connection,
                        usbd_device_instance_t* instance) {
  if (kAoaVid != instance->ident.vendor) return;
  LOG4CXX_INFO(logger_, "USB AOA device arrived (path "
                            << static_cast<int>(instance->path) << ", devno "
                            << static_cast<int>(instance->devno) << ", config "
                            << static_cast<int>(instance->config) << ", iface "
                            << static_cast<int>(instance->iface) << ")");
  usb_handler->DeviceArrived(connection, instance);
}

void LeftCallback(usbd_connection* connection,
                  usbd_device_instance_t* instance) {
  if (kAoaVid == instance->ident.vendor) return;
  LOG4CXX_INFO(logger_, "USB device left (path "
                            << static_cast<int>(instance->path) << ", devno "
                            << static_cast<int>(instance->devno) << ", config "
                            << static_cast<int>(instance->config) << ", iface "
                            << static_cast<int>(instance->iface) << ")");
  usb_handler->DeviceLeft(instance);
}

void LeftAoaCallback(usbd_connection* connection,
                     usbd_device_instance_t* instance) {
  if (kAoaVid != instance->ident.vendor) return;
  LOG4CXX_INFO(logger_, "USB AOA device left (path "
                            << static_cast<int>(instance->path) << ", devno "
                            << static_cast<int>(instance->devno) << ", config "
                            << static_cast<int>(instance->config) << ", iface "
                            << static_cast<int>(instance->iface) << ")");
  usb_handler->DeviceLeft(instance);
}

TransportAdapter::Error UsbHandler::Init() {
  {
    usbd_device_ident_t interest = {
        USBD_CONNECT_WILDCARD, USBD_CONNECT_WILDCARD, kAoaInterfaceSubclass,
        USBD_CONNECT_WILDCARD, USBD_CONNECT_WILDCARD};
    usbd_funcs_t funcs = {_USBDI_NFUNCS, ArrivedCallback, LeftCallback, NULL};
    usbd_connect_parm_t cparms = {NULL, USB_VERSION, USBD_VERSION, 0,     0,
                                  NULL, 0,           &interest,    &funcs};

    const int connect_rc = usbd_connect(&cparms, &usbd_general_connection_);

    if (EOK != connect_rc) {
      LOG4CXX_ERROR(logger_, "usbd_connect failed, error code " << connect_rc);
      return TransportAdapter::FAIL;
    }
  }
  {
    usbd_device_ident_t interest = {
        kAoaVid,               USBD_CONNECT_WILDCARD, USBD_CONNECT_WILDCARD,
        kAoaInterfaceSubclass, USBD_CONNECT_WILDCARD};
    usbd_funcs_t funcs = {_USBDI_NFUNCS,   ArrivedAoaCallback,
                          LeftAoaCallback, NULL};
    usbd_connect_parm_t cparms = {NULL, USB_VERSION, USBD_VERSION, 0,     0,
                                  NULL, 0,           &interest,    &funcs};

    const int connect_rc = usbd_connect(&cparms, &usbd_aoa_connection_);

    if (EOK != connect_rc) {
      LOG4CXX_ERROR(logger_, "usbd_connect failed, error code " << connect_rc);
      return TransportAdapter::FAIL;
    }
  }

  return TransportAdapter::OK;
}

}  // namespace
}  // namespace
