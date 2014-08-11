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

#include "transport_manager/aoa/aoa_wrapper.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

static void OnConnectedDevice(aoa_hdl_t *hdl, const void *udata) {
  LOG4CXX_DEBUG(logger_, "AOA: connected device " << hdl);
}

static void OnReceivedData(aoa_hdl_t *hdl, uint8_t *data, uint32_t sz,
                           uint32_t status, const void *udata) {
  LOG4CXX_DEBUG(logger_, "AOA: received data from device " << hdl);
  // read data here (received)
  // or
  // write to log (transmited)
}

AOAWrapper::AOAWrapper(AOAHandle hdl)
    : hdl_(hdl) {
}

bool AOAWrapper::Init() {
  LOG4CXX_TRACE(logger_, "AOA: init");
  int ret = aoa_init(NULL, NULL, &OnConnectedDevice, NULL,
                     AOA_FLAG_EXTERNAL_SWITCH);
  if (AOAWrapper::IsError(ret)) {
    AOAWrapper::PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::Subscribe() {
  LOG4CXX_TRACE(logger_, "AOA: subscribe " << hdl_ << " - "<< endpoint_);
  const void *udata;
  int ret = aoa_set_callback(hdl_, &OnReceivedData, udata, endpoint_);
  if (AOAWrapper::IsError(ret)) {
    AOAWrapper::PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::Shutdown() const {
  LOG4CXX_TRACE(logger_, "AOA: shutdown");
  int ret = aoa_shutdown();
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::IsValidHandle(AOAHandle hdl) {
  LOG4CXX_TRACE(logger_, "AOA: check handle " << hdl_);
  bool valid;
  int ret = aoa_get_valid(hdl, &valid);
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return valid;
}

bool AOAWrapper::IsValidHandle() const {
  return IsValidHandle(hdl_);
}

AOAVersion AOAWrapper::Version(uint16_t version) {
  switch (version) {
    case 1: return AOA_1_0;
    case 2: return AOA_2_0;
    default: return AOA_Unknown;
  }
}

AOAVersion AOAWrapper::GetProtocol(AOAHandle hdl) const {
  LOG4CXX_TRACE(logger_, "AOA: get protocol version for handle " << hdl_);
  uint16_t version;
  int ret = aoa_get_protocol(hdl, &version);
  if (IsError(ret)) {
    PrintError(ret);
  }
  return Version(version);
}

AOAVersion AOAWrapper::GetProtocol() const {
  return GetProtocol(hdl_);
}

uint32_t AOAWrapper::GetMaximumSizeBuffer() const {
  uint32_t size;
  int ret = aoa_get_endpoint_bufsz(hdl_, endpoint_, &size);
  if (IsError(ret)) {
    PrintError(ret);
  }
  return size;
}

bool AOAWrapper::SendToBulkEndpoint(RawMessagePtr message) const {
  LOG4CXX_TRACE(logger_, "AOA: send to bulk endpoint");
  int ret = aoa_bulk_tx(hdl_, endpoint_, timeout_, message->data(),
                        message->data_size());
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::SendToControlEndpoint(uint16_t request, uint16_t value,
                                       uint16_t index,
                                       RawMessagePtr message) const {
  LOG4CXX_TRACE(logger_, "AOA: send to control endpoint");
  int ret = aoa_control_tx(hdl_, endpoint_, timeout_, request, value, index,
                           message->data(), message->data_size());
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::IsError(int ret) const {
  return ret != AOA_EOK;
}

void AOAWrapper::PrintError(int ret) const {
  LOG4CXX_ERROR(logger_, "AOA: error " << ret << " - " << aoa_err2str(ret));
}

}  // namespace transport_adapter
}  // namespace transport_manager
