/*
 * \file usb_aoa_adapter.cpp
 * \brief UsbAoaAdapter class source file.
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

#include "utils/logger.h"
#include "transport_manager/usb/usb_aoa_adapter.h"
#include "transport_manager/usb/usb_device_scanner.h"
#include "transport_manager/usb/usb_connection_factory.h"
#include "transport_manager/usb/common.h"
#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "UsbTransportAdapter")
#ifdef SP_C9_PRIMA1
UsbAoaAdapter* UsbAoaAdapter::g_pUsbAoaAdapter_ = NULL;
void DeviceConnect(void)
{
  LOG4CXX_INFO(logger_, "sp_c9_prima1 device connect. ");
  PRINTMSG(1, (L"\nsp_c9_prima1 %s, line:%d\n", __FUNCTIONW__, __LINE__));
  UsbAoaAdapter::GetInstance()->GetUsbHandlerSptr()->DeviceArrived(NULL);
  UsbAoaAdapter::GetInstance()->GetSkEAHelperDLL()->SP_EAHelperLaunchSDL();
	LOG4CXX_INFO(logger_, "pEAHelperDLL_->SP_EAHelperLaunchSDL()");
  PRINTMSG(1, (L"\nsp_c9_prima1 %s, line:%d\n", __FUNCTIONW__, __LINE__));
	
}

void DeviceDisconnect(void)
{
	LOG4CXX_INFO(logger_, "sp_c9_prima1 device disconnect. ");
  PRINTMSG(1, (L"\nsp_c9_prima1 %s, line:%d\n", __FUNCTIONW__, __LINE__));
	UsbAoaAdapter::GetInstance()->GetUsbHandlerSptr()->DeviceLeft(NULL);
	UsbAoaAdapter::GetInstance()->DisconnectDone("1", 1);
}

void ReceiveData(const BYTE* chBuff, int iBufLen)
{
	LOG4CXX_INFO(logger_, "sp_c9_prima1 receive data. buflen is " << iBufLen);
    RawMessageSptr data(new protocol_handler::RawMessage(
        0, 0, (uint8_t*)chBuff, iBufLen));
    UsbAoaAdapter::GetInstance()->DataReceiveDone("1", 1, data);
}
#endif

UsbAoaAdapter::UsbAoaAdapter()
  : TransportAdapterImpl(new UsbDeviceScanner(this),
                         new UsbConnectionFactory(this), 0),
  is_initialised_(false),
  usb_handler_(new UsbHandler()) {
#ifdef SP_C9_PRIMA1
  pEAHelperDLL_ = new SkEAHelperDLL;
#endif
  static_cast<UsbDeviceScanner*>(device_scanner_)->SetUsbHandler(usb_handler_);
  static_cast<UsbConnectionFactory*>(server_connection_factory_)
  ->SetUsbHandler(usb_handler_);
}

UsbAoaAdapter::~UsbAoaAdapter() {
#ifdef SP_C9_PRIMA1
	if(pEAHelperDLL_ != NULL){
		delete pEAHelperDLL_;
		pEAHelperDLL_ = NULL;
	}
#endif
}

DeviceType UsbAoaAdapter::GetDeviceType() const {
	return PASA_AOA;
}

bool UsbAoaAdapter::IsInitialised() const {
  return is_initialised_ && TransportAdapterImpl::IsInitialised();
}

TransportAdapter::Error UsbAoaAdapter::Init() {
#ifdef SP_C9_PRIMA1
	bool ret = pEAHelperDLL_->SP_EAHelperInit(DeviceConnect, DeviceDisconnect, ReceiveData);
	LOG4CXX_INFO(logger_, "sp_c9_prima1 init, result is " << ret);
  PRINTMSG(1, (L"\n%s, line:%d, sp_c9_prima1 init()\n", __FUNCTIONW__, __LINE__));
	if(!ret){
		LOG4CXX_INFO(logger_, "sp_c9_prima1 init failed");
		PRINTMSG(1, (L"\n%s, line:%d, sp_c9_prima1 init() fail\n", __FUNCTIONW__, __LINE__));
		return FAIL;
	}
#endif
  TransportAdapter::Error error = usb_handler_->Init();
  if (error != TransportAdapter::OK) {
    LOG4CXX_TRACE(logger_, "exit with error " << error <<
                  ". Condition: error != TransportAdapter::OK");
    return error;
  }
  error = TransportAdapterImpl::Init();
  if (error != TransportAdapter::OK) {
    LOG4CXX_TRACE(logger_, "exit with error " << error <<
                  ". Condition: error != TransportAdapter::OK");
    return error;
  }
  is_initialised_ = true;
  LOG4CXX_TRACE(logger_, "exit with TransportAdapter::OK");
  return TransportAdapter::OK;
}

bool UsbAoaAdapter::ToBeAutoConnected(DeviceSptr device) const {
  return true;
}

#ifdef SP_C9_PRIMA1
TransportAdapter::Error UsbAoaAdapter::SendData(
    const DeviceUID& device_id, const ApplicationHandle& app_handle,
    const RawMessageSptr data) {
  if (!is_initialised_) return BAD_STATE;

  ConnectionSptr connection = FindEstablishedConnection(device_id, app_handle);
  if (connection.get() != 0) {
	  pEAHelperDLL_->SP_EAHelperWrite(data->data(), data->data_size());
		LOG4CXX_INFO(logger_, "sp_c9_prima1 send data. buflen is " << data->data_size());
	  return OK;
  } else {
    return BAD_PARAM;
  }
}
#endif
}  // namespace transport_adapter
}  // namespace transport_manager
