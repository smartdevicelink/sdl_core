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

#include "transport_manager/aoa/pps_device_scanner.h"

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#include <aoa/aoa.h>
#include <sys/pps.h>
#include <sys/iomsg.h>
#include <sys/neutrino.h>

#include "utils/logger.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/aoa/aoa_dynamic_device.h"

#define REQUEST_MASK    (AOA_MODE_ACCESSORY | AOA_MODE_AUDIO)

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

enum {
  ATTR_AOA = 0,
  ATTR_BUSNO,
  ATTR_DEVNO,
  ATTR_MANUFACTURER,
  ATTR_VENDOR_ID,
  ATTR_PRODUCT,
  ATTR_PRODUCT_ID,
  ATTR_SERIAL_NUMBER,

  ATTR_COUNT
};

/* The keys required by usblauncher to send to the device */
static const char *keys[] = { "manufacturer", "model", "description", "version",
    "uri", "serial",

    NULL };

/* The values used to switch the phone into AOA mode */
static const char *values[] = { "QNX Multimedia", "QNX", "2.0",
    "QNX AOA Demo Application", "http://www.qnx.com", "12345678",

    NULL };

const std::string PPSDeviceScanner::kUSBStackPath = "/dev/io-usb/io-usb";
const std::string PPSDeviceScanner::kPpsPathRoot = "/pps/qnx/device/";
const std::string PPSDeviceScanner::kPpsPathAll = ".all";
const std::string PPSDeviceScanner::kPpsPathCtrl = "usb_ctrl1";

PPSDeviceScanner::PPSDeviceScanner(TransportAdapterController* controller)
    : initialised_(false),
      controller_(controller),
      fd_(-1),
      thread_(new threads::Thread("PPS scanner", new PpsThreadDelegate(this))) {
}

PPSDeviceScanner::~PPSDeviceScanner() {
  Terminate();
  delete thread_;
}

TransportAdapter::Error PPSDeviceScanner::Init() {
  initialised_ = OpenPps();
  thread_->start();
  return (initialised_) ? TransportAdapter::OK : TransportAdapter::FAIL;
}

TransportAdapter::Error PPSDeviceScanner::Scan() {
  return TransportAdapter::NOT_SUPPORTED;
}

void PPSDeviceScanner::Terminate() {
  initialised_ = false;
  thread_->stop();
  ClosePps();
}

bool PPSDeviceScanner::IsInitialised() const {
  return initialised_;
}

bool PPSDeviceScanner::OpenPps() {
  LOG4CXX_TRACE(logger_, "AOA: open pps");
  const std::string kPath = kPpsPathRoot + kPpsPathAll;
  if ((fd_ = open(kPath.c_str(), O_RDONLY)) == -1) {
    LOG4CXX_ERROR(
        logger_,
        "AOA: error opening file '" << kPath << "': (" << strerror(errno) << ")");
    return false;
  }
  return true;
}

bool PPSDeviceScanner::ArmEvent(struct sigevent* event) {
  LOG4CXX_TRACE(logger_, "AOA: arm event");
  uint8_t buf[2048];
  while (ionotify(fd_, _NOTIFY_ACTION_POLLARM, _NOTIFY_COND_INPUT, event)
      & _NOTIFY_COND_INPUT) {
    int size = read(fd_, buf, sizeof(buf));
    if (size > 0) {
      buf[size] = '\0';
      Process(buf, size);
    }
  }
  return true;
}

std::string PPSDeviceScanner::ParsePps(char* ppsdata, const char** vals) {
  LOG4CXX_TRACE(logger_, "AOA: parse pps data");
  static const char* attrs[] = { "aoa", "busno", "devno", "manufacturer",
      "vendor_id", "product", "product_id", "serial_number", NULL };
  int rc;
  std::string object_name;
  pps_attrib_t info = { 0 };
  /* Loop through the PPS objects looking for aoa, busno, devno and etc. */
  while ((rc = ppsparse(&ppsdata, NULL, attrs, &info, 0))) {
    if (rc == PPS_OBJECT) {
      if (info.obj_name) {
        object_name = info.obj_name;
      }
      LOG4CXX_DEBUG(logger_, "AOA: object name " << object_name);
    } else if (rc == PPS_ATTRIBUTE && info.attr_index != -1) {
      LOG4CXX_DEBUG(
          logger_, "AOA: index " << info.attr_index << ", value " << info.value);
      /* If our value matches our attribute index, keep a pointer to it */
      vals[info.attr_index] = info.value;
    }
  }

  return object_name;
}

void PPSDeviceScanner::Process(uint8_t* buf, size_t size) {
  LOG4CXX_TRACE(logger_, "AOA: process pps");
  if (!buf) {
    return;
  }

  char* ppsdata = reinterpret_cast<char*>(buf);
  const char* vals[ATTR_COUNT] = { 0 };

  std::string object_name = ParsePps(ppsdata, vals);
  if (CheckData(vals)) {
    AOAWrapper::AOAUsbInfo aoa_usb_info;
    FillUsbInfo(object_name, vals, &aoa_usb_info);
    WritePpsData(object_name.c_str(), vals);
    AddDevice(aoa_usb_info);
  }
}

bool PPSDeviceScanner::CheckData(const char** attrs) {
  LOG4CXX_TRACE(logger_, "AOA: check data about device");
  /* Make sure we support AOA and have all fields */
  for (int i = 0; i < ATTR_COUNT; i++) {
    if (!attrs[i]) {
      return false;
    }
  }
  return true;
}

void PPSDeviceScanner::FillUsbInfo(const std::string& object_name,
                                   const char** attrs,
                                   AOAWrapper::AOAUsbInfo* info) {
  info->path = kUSBStackPath;
  info->aoa_version = strtoul(attrs[ATTR_AOA], NULL, 0);
  info->busno = strtoul(attrs[ATTR_BUSNO], NULL, 0);
  info->devno = strtoul(attrs[ATTR_DEVNO], NULL, 0);
  info->manufacturer = attrs[ATTR_MANUFACTURER];
  info->vendor_id = strtoul(attrs[ATTR_VENDOR_ID], NULL, 0);
  info->product = attrs[ATTR_PRODUCT];
  info->product_id = strtoul(attrs[ATTR_PRODUCT_ID], NULL, 0);
  info->serial_number = attrs[ATTR_SERIAL_NUMBER];
  info->iface = 0;
}

void PPSDeviceScanner::WritePpsData(const char* objname, const char** attrs) {
  LOG4CXX_TRACE(logger_, "AOA: Write pps to device");
  int i, rc, fd;
  char cmd[PATH_MAX];
  size_t sz;

  /* Do we have an object name? */
  if (!objname) {
    return;
  }

  /* Create the PPS request string (start_aoa::busno,devno,request_mask,key=value,key=value,...) */
  sz = snprintf(cmd, sizeof(cmd), "start_aoa::busno=%s,devno=%s,modes=%u",
                attrs[ATTR_BUSNO], attrs[ATTR_DEVNO], REQUEST_MASK);

  i = 0;
  while (keys[i]) {
    /* Add all the key/value pairs to the request */
    sz += snprintf(cmd + sz, sizeof(cmd) - sz, ",%s=%s", keys[i], values[i]);
    i++;
  }

  /* Open the object we want to write to */
  std::string ctrl = kPpsPathRoot + kPpsPathCtrl;
  fd = open(ctrl.c_str(), O_WRONLY);
  if (fd == -1) {
    LOG4CXX_ERROR(
        logger_,
        "AOA: Error opening file '" << ctrl << "': " << strerror(errno) << "'");
    return;
  }

  /* Write to the PPS object */
  rc = write(fd, cmd, sz);
  if (rc == -1) {
    LOG4CXX_ERROR(
        logger_,
        "AOA: Error writing to file '" << ctrl << "': " << strerror(errno) << "' " << cmd);
    return;
  }

  close(fd);
}

void PPSDeviceScanner::ClosePps() {
  LOG4CXX_TRACE(logger_, "AOA: close pps");
  close(fd_);
  fd_ = -1;
}

void PPSDeviceScanner::AddDevice(const AOAWrapper::AOAUsbInfo& aoa_usb_info) {
  LOG4CXX_TRACE(logger_, "AOA: add device");
  AOADevicePtr aoa_device(
      new AOADynamicDevice(aoa_usb_info.product, aoa_usb_info.serial_number,
                           aoa_usb_info, controller_));
  sync_primitives::AutoLock loker(devices_lock_);
  devices_.insert(std::make_pair(aoa_usb_info.serial_number, aoa_device));
}

void PPSDeviceScanner::NotifyDevicesUpdated() {
  LOG4CXX_TRACE(logger_, "AOA: notify devices update");
  DeviceVector devices;
  devices_lock_.Acquire();
  for (DeviceContainer::const_iterator i = devices_.begin();
      i != devices_.end(); ++i) {
    AOADevicePtr aoa_device = i->second;
    DeviceSptr device = AOADevicePtr::static_pointer_cast<Device>(aoa_device);
    devices.push_back(device);
  }
  devices_lock_.Release();
  controller_->SearchDeviceDone(devices);
}

PPSDeviceScanner::PpsThreadDelegate::PpsThreadDelegate(PPSDeviceScanner* parent)
    : parent_(parent) {
}

bool PPSDeviceScanner::PpsThreadDelegate::ArmEvent(struct sigevent* event) {
  return parent_->ArmEvent(event);
}

void PPSDeviceScanner::PpsThreadDelegate::OnPulse() {
}

}  // namespace transport_adapter
}  // namespace transport_manager

