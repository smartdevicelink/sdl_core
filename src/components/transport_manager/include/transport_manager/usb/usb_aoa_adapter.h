/*
 * \file usb_aoa_adapter.h
 * \brief UsbAoaAdapter class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_USB_AOA_ADAPTER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_USB_AOA_ADAPTER_H_

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/usb/common.h"
#ifdef SP_C9_PRIMA1
#include "transport_manager/usb/SkEAHelper_DLL.h"
#endif

namespace transport_manager {
namespace transport_adapter {

class UsbAoaAdapter : public TransportAdapterImpl {
 public:
  UsbAoaAdapter();
  virtual ~UsbAoaAdapter();
#ifdef SP_C9_PRIMA1
  static UsbAoaAdapter* GetInstance()
  {
	  if(g_pUsbAoaAdapter_ == NULL){
		g_pUsbAoaAdapter_ = new UsbAoaAdapter();
	  }
	  return g_pUsbAoaAdapter_;
  }

  static void DelInstance(){
	  if(g_pUsbAoaAdapter_ != NULL){
		delete g_pUsbAoaAdapter_;
	  }
  }
	
  UsbHandlerSptr GetUsbHandlerSptr(){
	return usb_handler_;
  }

  SkEAHelperDLL* GetSkEAHelperDLL(){
	return pEAHelperDLL_;
  }
#endif

 protected:
  virtual DeviceType GetDeviceType() const;
  virtual bool IsInitialised() const;
  virtual TransportAdapter::Error Init();
  virtual bool ToBeAutoConnected(DeviceSptr device) const;
#ifdef SP_C9_PRIMA1
  virtual TransportAdapter::Error SendData(const DeviceUID& device_handle,
                                           const ApplicationHandle& app_handle,
                                           const RawMessageSptr data);
#endif
 private:
  bool is_initialised_;
  UsbHandlerSptr usb_handler_;
#ifdef SP_C9_PRIMA1
	 static UsbAoaAdapter* g_pUsbAoaAdapter_;
	SkEAHelperDLL* pEAHelperDLL_;
#endif
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_USB_AOA_ADAPTER
