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

#include <aoa/aoa.h>

#include "utils/macro.h"
#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

static void OnConnectedDevice(aoa_hdl_t *hdl, const void *udata) {
  LOG4CXX_DEBUG(logger_, "AOA: connected device " << hdl);
  AOALifeDevice* const * p = static_cast<AOALifeDevice* const *>(udata);
  AOALifeDevice* life = *p;
  life->Loop(hdl);
}

static void OnReceivedData(aoa_hdl_t *hdl, uint8_t *data, uint32_t sz,
                           uint32_t status, const void *udata) {
  LOG4CXX_DEBUG(logger_, "AOA: received data from device " << hdl);
  bool error = AOAWrapper::IsError(status);
  if (error) {
    AOAWrapper::PrintError(status);
  }

  bool success = !error;
  RawMessagePtr message;
  if (data) {
    message = new RawMessage(0, 0, data, sz);
  } else {
    LOG4CXX_ERROR(logger_, "AOA: data is null");
    success = false;
  }

  AOAConnectionObserver* const * p = static_cast<AOAConnectionObserver* const *>(udata);
  AOAConnectionObserver* observer = *p;
  observer->OnMessageReceived(success, message);

  if (!AOAWrapper::IsHandleValid(hdl)) {
    observer->OnDisconnected();
    AOAWrapper::Died(hdl);
  }
}

static void OnTransmittedData(aoa_hdl_t *hdl, uint8_t *data, uint32_t sz,
                              uint32_t status, const void *udata) {
  LOG4CXX_DEBUG(logger_, "AOA: transmitted data to device " << hdl << ", size=" << sz << ", data=" << data);
  bool error = AOAWrapper::IsError(status);
  if (error) {
    AOAWrapper::PrintError(status);
  }

  bool success = !error;
  RawMessagePtr message;
  if (data) {
    message = new RawMessage(0, 0, data, sz);
  } else {
    // TODO(KKolodiy): data is allways null now
    LOG4CXX_ERROR(logger_, "AOA: data is null");
    success = false;
  }

  AOAConnectionObserver* const * p = static_cast<AOAConnectionObserver* const *>(udata);
  AOAConnectionObserver* observer = *p;
  observer->OnMessageTransmitted(success, message);

  if (!AOAWrapper::IsHandleValid(hdl)) {
    observer->OnDisconnected();
    AOAWrapper::Died(hdl);
  }
}

AOALifeDevice* AOAWrapper::life_ = 0;

AOAWrapper::AOAWrapper(AOAHandle hdl)
    : hdl_(hdl),
      timeout_(AOA_TIMEOUT_INFINITY),
      connection_observer_(0) {
}

AOAWrapper::AOAWrapper(AOAHandle hdl, uint32_t timeout)
    : hdl_(hdl),
      timeout_(timeout),
      connection_observer_(0) {
}

bool AOAWrapper::Init(AOALifeDevice *life) {
  LOG4CXX_TRACE(logger_, "AOA: init default");
  life_ = life;
  int ret = aoa_init(NULL, NULL, &OnConnectedDevice, &life_, 0);
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::Init(const std::string& path_to_config,
                      AOALifeDevice* life) {
  LOG4CXX_TRACE(logger_, "AOA: init default usb_info");
  life_ = life;
  int ret = aoa_init(path_to_config.c_str(), NULL, &OnConnectedDevice,
                     &life_, 0);
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::Init(const AOAWrapper::AOAUsbInfo& aoa_usb_info,
                      AOALifeDevice* life) {
  LOG4CXX_TRACE(logger_, "AOA: init default path to config");
  life_ = life;
  usb_info_t usb_info;
  PrepareUsbInfo(aoa_usb_info, &usb_info);
  int ret = aoa_init(NULL, &usb_info, &OnConnectedDevice, &life_,
                     AOA_FLAG_UNIQUE_DEVICE);
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::Init(const std::string& path_to_config,
                      const AOAWrapper::AOAUsbInfo& aoa_usb_info,
                      AOALifeDevice *life) {
  LOG4CXX_TRACE(logger_, "AOA: init");
  life_ = life;
  usb_info_t usb_info;
  PrepareUsbInfo(aoa_usb_info, &usb_info);
  int ret = aoa_init(path_to_config.c_str(), &usb_info, &OnConnectedDevice,
                     &life_, AOA_FLAG_UNIQUE_DEVICE);
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::SetCallback(AOAEndpoint endpoint) const {
  LOG4CXX_TRACE(logger_,
                "AOA: set callback " << hdl_ << ", endpoint " << endpoint);
  data_clbk_t callback;
  switch (endpoint) {
    case AOA_Ept_Accessory_BulkIn:
      callback = &OnReceivedData;
      break;
    case AOA_Ept_Accessory_BulkOut:
      callback = &OnTransmittedData;
      break;
    default:
      LOG4CXX_ERROR(
          logger_, "AOA: " << endpoint << " endpoint doesn't support callbacks")
      ;
      return false;
  }
  int ret = aoa_set_callback(hdl_, callback, &connection_observer_,
                             BitEndpoint(endpoint));
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::Subscribe(AOAConnectionObserver *observer) {
  LOG4CXX_TRACE(logger_, "AOA: subscribe on receive data " << hdl_);
  connection_observer_ = observer;
  if (!SetCallback(AOA_Ept_Accessory_BulkIn)) {
    return false;
  }
  ReceiveMessage();

  // TODO(KKolodiy): callback return data is null now
//  LOG4CXX_TRACE(logger_, "AOA: subscribe on transmit data " << hdl_);
//  if (!SetCallback(AOA_Ept_Accessory_BulkOut)) {
//    return false;
//  }
  return true;
}

bool AOAWrapper::UnsetCallback(AOAEndpoint endpoint) const {
  int ret_r = aoa_set_callback(hdl_, NULL, NULL, BitEndpoint(endpoint));
  if (IsError(ret_r)) {
    PrintError(ret_r);
    return false;
  }
  return true;
}

bool AOAWrapper::Unsubscribe() {
  LOG4CXX_TRACE(logger_, "AOA: unsubscribe on receive data" << hdl_);
  if (!UnsetCallback(AOA_Ept_Accessory_BulkIn)) {
    return false;
  }
  LOG4CXX_TRACE(logger_, "AOA: unsubscribe on transmit data" << hdl_);
  if (!UnsetCallback(AOA_Ept_Accessory_BulkOut)) {
    return false;
  }
  connection_observer_ = 0;
  return true;
}

bool AOAWrapper::Shutdown() {
  LOG4CXX_TRACE(logger_, "AOA: shutdown");
  int ret = aoa_shutdown();
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  life_ = 0;
  return true;
}

bool AOAWrapper::IsHandleValid() const {
  return IsHandleValid(hdl_);
}

AOAVersion AOAWrapper::Version(uint16_t version) const {
  switch (version) {
    case 1:
      return AOA_Ver_1_0;
    case 2:
      return AOA_Ver_2_0;
    default:
      return AOA_Ver_Unknown;
  }
}

AOAVersion AOAWrapper::GetProtocolVesrion() const {
  LOG4CXX_TRACE(logger_, "AOA: get protocol version for handle " << hdl_);
  uint16_t version;
  int ret = aoa_get_protocol(hdl_, &version);
  if (IsError(ret)) {
    PrintError(ret);
  }
  return Version(version);
}

uint32_t AOAWrapper::BitEndpoint(AOAEndpoint endpoint) const {
  const uint32_t kUndefined = 0;
  switch (endpoint) {
    case AOA_Ept_Accessory_BulkIn:
      return AOA_EPT_ACCESSORY_BULKIN;
    case AOA_Ept_Accessory_BulkOut:
      return AOA_EPT_ACCESSORY_BULKOUT;
    case AOA_Ept_Accessory_Control:
      return AOA_EPT_ACCESSORY_CONTROL;
    default:
      return kUndefined;
  }
}

uint32_t AOAWrapper::GetBufferMaximumSize(AOAEndpoint endpoint) const {
  uint32_t size;
  int ret = aoa_get_endpoint_bufsz(hdl_, BitEndpoint(endpoint), &size);
  if (IsError(ret)) {
    PrintError(ret);
  }
  return size;
}

bool AOAWrapper::IsValueInMask(uint32_t bitmask, uint32_t value) const {
  return (bitmask & value) == value;
}

std::vector<AOAMode> AOAWrapper::CreateModesList(uint32_t modes_mask) const {
  std::vector<AOAMode> list;
  if (IsValueInMask(modes_mask, AOA_MODE_ACCESSORY)) {
    list.push_back(AOA_Mode_Accessory);
  }
  if (IsValueInMask(modes_mask, AOA_MODE_AUDIO)) {
    list.push_back(AOA_Mode_Audio);
  }
  if (IsValueInMask(modes_mask, AOA_MODE_DEBUG)) {
    list.push_back(AOA_Mode_Debug);
  }
  return list;
}

std::vector<AOAMode> AOAWrapper::GetModes() const {
  uint32_t modes;
  int ret = aoa_get_mode_mask(hdl_, &modes);
  if (IsError(ret)) {
    PrintError(ret);
  }
  return CreateModesList(modes);
}

std::vector<AOAEndpoint> AOAWrapper::CreateEndpointsList(
    uint32_t endpoints_mask) const {
  std::vector<AOAEndpoint> list;
  if (IsValueInMask(endpoints_mask, AOA_EPT_ACCESSORY_BULKIN)) {
    list.push_back(AOA_Ept_Accessory_BulkIn);
  }
  if (IsValueInMask(endpoints_mask, AOA_EPT_ACCESSORY_BULKOUT)) {
    list.push_back(AOA_Ept_Accessory_BulkOut);
  }
  if (IsValueInMask(endpoints_mask, AOA_EPT_ACCESSORY_CONTROL)) {
    list.push_back(AOA_Ept_Accessory_Control);
  }
  return list;
}

std::vector<AOAEndpoint> AOAWrapper::GetEndpoints() const {
  uint32_t endpoints;
  int ret = aoa_get_endpoint_mask(hdl_, &endpoints);
  if (IsError(ret)) {
    PrintError(ret);
  }
  return CreateEndpointsList(endpoints);
}

bool AOAWrapper::SendMessage(RawMessagePtr message) const {
  LOG4CXX_TRACE(logger_, "AOA: send to bulk endpoint");
  DCHECK(message);

  if (!IsHandleValid()) {
    connection_observer_->OnDisconnected();
    Died(hdl_);
    return false;
  }

  uint8_t *data = message->data();
  size_t length = message->data_size();
  int ret = aoa_bulk_tx(hdl_, AOA_EPT_ACCESSORY_BULKOUT, timeout_, data,
                        &length);
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

bool AOAWrapper::SendControlMessage(uint16_t request, uint16_t value,
                                    uint16_t index,
                                    RawMessagePtr message) const {
  LOG4CXX_TRACE(logger_, "AOA: send to control endpoint");
  DCHECK(message);

  if (!IsHandleValid()) {
    connection_observer_->OnDisconnected();
    Died(hdl_);
    return false;
  }

  uint8_t *data = message->data();
  size_t length = message->data_size();
  int ret = aoa_control_tx(hdl_, AOA_EPT_ACCESSORY_CONTROL, timeout_, request,
                           value, index, data, &length);
  if (IsError(ret)) {
    PrintError(ret);
    return false;
  }
  return true;
}

RawMessagePtr AOAWrapper::ReceiveMessage() const {
  LOG4CXX_TRACE(logger_, "AOA: receive from endpoint");

  if (!IsHandleValid()) {
    connection_observer_->OnDisconnected();
    Died(hdl_);
    return false;
  }

  uint8_t *data;
  uint32_t size;
  int ret = aoa_bulk_rx(hdl_, AOA_EPT_ACCESSORY_BULKIN, timeout_, &data, &size);
  if (IsError(ret)) {
    PrintError(ret);
  } else if (data) {
    return RawMessagePtr(new RawMessage(0, 0, data, size));
  }
  return RawMessagePtr();
}

RawMessagePtr AOAWrapper::ReceiveControlMessage(uint16_t request,
                                                uint16_t value,
                                                uint16_t index) const {
  LOG4CXX_TRACE(logger_, "AOA: receive from control endpoint");

  if (!IsHandleValid()) {
    connection_observer_->OnDisconnected();
    Died(hdl_);
    return false;
  }

  uint8_t *data;
  uint32_t size;
  int ret = aoa_control_rx(hdl_, AOA_EPT_ACCESSORY_CONTROL, timeout_, request,
                           value, index, &data, &size);
  if (IsError(ret)) {
    PrintError(ret);
  } else if (data) {
    return RawMessagePtr(new RawMessage(0, 0, data, size));
  }
  return RawMessagePtr();
}

bool AOAWrapper::IsError(int ret) {
  return ret != AOA_EOK;
}

void AOAWrapper::PrintError(int ret) {
  LOG4CXX_ERROR(logger_, "AOA: error " << ret << " - " << aoa_err2str(ret));
}

bool AOAWrapper::IsHandleValid(AOAWrapper::AOAHandle hdl) {
  LOG4CXX_TRACE(logger_, "AOA: check handle " << hdl);
  bool valid;
  int ret = aoa_get_valid(hdl, &valid);
  if (IsError(ret)) {
    PrintError(ret);
    valid = false;
  }
  return valid;
}

void AOAWrapper::Died(AOAWrapper::AOAHandle hdl) {
  life_->Died(hdl);
}

void AOAWrapper::PrepareUsbInfo(const AOAUsbInfo& aoa_usb_info,
                                usb_info_s* usb_info) {
  usb_info->path = aoa_usb_info.path.c_str();
  usb_info->busno = aoa_usb_info.busno;
  usb_info->devno = aoa_usb_info.devno;
  usb_info->iface = aoa_usb_info.iface;
}

}  // namespace transport_adapter
}  // namespace transport_manager

