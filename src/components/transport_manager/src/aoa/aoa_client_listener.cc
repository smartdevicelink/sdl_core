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

#include "transport_manager/aoa/aoa_client_listener.h"

#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/iomsg.h>
//#include <sys/pps.h>

#include "utils/logger.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

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

const std::string AOAClientListener::kPpsPathRoot = "/pps/qnx/device/";
const std::string AOAClientListener::kPpsPathAll = ".all";
const std::string AOAClientListener::kPpsPathCtrl = "usb_ctrl";

AOAClientListener::AOAClientListener(TransportAdapterController* controller)
    : controller_(controller),
      fd_(-1) {
}

TransportAdapter::Error AOAClientListener::Init() {
  return TransportAdapter::OK;

}

void AOAClientListener::Terminate() {

}

bool AOAClientListener::IsInitialised() const {
  return true;
}

TransportAdapter::Error AOAClientListener::StartListening() {
  return TransportAdapter::OK;
}

TransportAdapter::Error AOAClientListener::StopListening() {
  return TransportAdapter::OK;
}

bool AOAClientListener::OpenPps() {
  const std::string kPath = kPpsPathRoot + kPpsPathAll;
  if ((fd_ = open(kPath.c_str(), O_RDONLY)) == -1) {
    LOG4CXX_ERROR(
        logger_,
        "AOA: error opening file '" << kPath << "': (" << strerror(errno) << ")");
    return false;
  }
  return true;
}

bool AOAClientListener::ArmEvent(struct sigevent* event) {
  uint8_t buf[2048];
  while (ionotify(fd_, _NOTIFY_ACTION_POLLARM, _NOTIFY_COND_INPUT, event)
      & _NOTIFY_COND_INPUT) {
    int size = read(fd_, buf, sizeof(buf));
    if (size > 0) {
      buf[size] = '\0';
      ProcessPpsData(buf, size);
    }
  }
  return true;
}

void AOAClientListener::ProcessPpsData(uint8_t* buf, size_t size) {
//  static const char* attrs[] = { "aoa", "busno", "devno", "manufacturer",
//      "vendor_id", "product", "product_id", "serial_number", NULL };
//  pps_attrib_t info = { 0 };
//  const char* vals[ATTR_COUNT] = { 0 };
//  char* ppsdata = static_cast<char*>(buf);
//  char* objname = NULL;
//  int rc;
//
//  if (!buf) {
//    return;
//  }
//
//  AOAWrapper::AOAUsbInfo aoa_info;
//  /* Loop through the PPS objects looking for aoa, busno, devno, vendor_id */
//  while ((rc = ppsparse(&ppsdata, NULL, attrs, &info, 0))) {
//    if (rc == PPS_OBJECT) {
//      objname = info.obj_name + 1;
//    } else if (rc == PPS_ATTRIBUTE && info.attr_index != -1) {
//      /* If our value matches our attribute index, keep a pointer to it */
//      vals[info.attr_index] = info.value;
//    }
//  }
//  FillUsbInfo(objname, vals, &aoa_info);
//  //WritePpsData(objname, vals);
}

void AOAClientListener::FillUsbInfo(const char* objname, const char** attrs,
                                    AOAWrapper::AOAUsbInfo* info) {
  info->path = kPpsPathRoot + "/" + objname;
  info->aoa_version = atoi(attrs[ATTR_AOA]);
  info->busno = atoi(attrs[ATTR_BUSNO]);
  info->devno = atoi(attrs[ATTR_DEVNO]);
  info->manufacturer = attrs[ATTR_MANUFACTURER];
  info->vendor_id = atoi(attrs[ATTR_VENDOR_ID]);
  info->product = attrs[ATTR_PRODUCT];
  info->product_id = atoi(attrs[ATTR_PRODUCT_ID]);
  info->serial_number = attrs[ATTR_SERIAL_NUMBER];
  info->iface = 0;
}

void AOAClientListener::WritePpsData(const char* objname, const char** attrs) {
//  int i, rc, fd;
//    char  cmd[PATH_MAX];
//    size_t  sz;
//
//    /* Do we have an object name? */
//    if (!objname) {
//      return;
//    }
//
//    /* Make sure we support AOA and have a busno, devno, vendor, and product ID */
//    for ( i=0; i<ATTR_COUNT; i++ ) {
//      if ( !attrs[i] ) {
//        return;
//      }
//    }
//
//    /*
//     * Ignore a device if it's already in AOA mode (product and vendor ID's
//     * will match a known list).
//     */
//    if ( stricmp(attrs[ATTR_VENDID], VENDOR_ID_GOOGLE) == 0 ) {
//      for ( i=0; prodlist[i]; i++ ) {
//        if ( stricmp(attrs[ATTR_PRODID], prodlist[i]) == 0 ) {
//          return;
//        }
//      }
//    }
//
//    /* Create the PPS request string (start_aoa::busno,devno,request_mask,key=value,key=value,...) */
//    sz = snprintf( cmd, sizeof(cmd), "start_aoa::busno=%s,devno=%s,modes=%u", attrs[ATTR_BUSNO], attrs[ATTR_DEVNO], REQUEST_MASK );
//
//    i=0;
//    while ( keys[i] ) {
//      /* Add all the key/value pairs to the request */
//      sz += snprintf( cmd + sz, sizeof(cmd) - sz, ",%s=%s", keys[i], values[i] );
//      i++;
//    }
//
//    /* Open the object we want to write to */
//    fd = open( PPS_PATH_ROOT PPS_PATH_CTRL, O_WRONLY );
//    if ( fd == -1 ) {
//      fprintf( stderr, "Error opening file '%s': (%s)\n", PPS_PATH_ROOT PPS_PATH_CTRL, strerror(errno) );
//      return;
//    }
//
//    /* Write to the PPS object */
//    rc = write( fd, cmd, sz );
//    if ( rc == -1 ) {
//      fprintf( stderr, "Error writing to file '%s': (%s)\n%s\n", PPS_PATH_ROOT PPS_PATH_CTRL, strerror(errno), cmd );
//      return;
//    }
//
//    close( fd );
}

void AOAClientListener::ClosePps() {
  LOG4CXX_TRACE(logger_, "AOA: finalize pps listener");
  close(fd_);
  fd_ = -1;
}

AOAClientListener::PpsThreadDelegate::PpsThreadDelegate(
    AOAClientListener* parent)
    : parent_(parent) {
}

bool AOAClientListener::PpsThreadDelegate::Init() {
  return parent_->OpenPps();
}

bool AOAClientListener::PpsThreadDelegate::ArmEvent(struct sigevent* event) {
  return parent_->ArmEvent(event);
}

void AOAClientListener::PpsThreadDelegate::OnPulse() {
}

void AOAClientListener::PpsThreadDelegate::Finalize() {
  parent_->ClosePps();
}

}  // namespace transport_adapter
}  // namespace transport_manager

