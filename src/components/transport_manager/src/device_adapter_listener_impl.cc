/**
 * \file device_adapter_listener_impl.cc
 * \brief Listener class source file.
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

#include "transport_manager/device_adapter_listener.h"


namespace transport_manager {


log4cxx::LoggerPtr DeviceAdapterListenerImpl::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger( "DeviceAdapterListener"));

void DeviceAdapterListenerImpl::OnSearchDeviceDone(const DeviceAdapter* device_adapter){

}

void DeviceAdapterListenerImpl::OnSearchDeviceFailed(const DeviceAdapter* device_adapter, const SearchDeviceError& error){

}

void DeviceAdapterListenerImpl::OnDeviceConnectDone(const DeviceAdapter* device_adapter, DeviceHandle device_handle){

}

void DeviceAdapterListenerImpl::OnDeviceConnectFailed(const DeviceAdapter* device_adapter, DeviceHandle device_handle, const ConnectDeviceError& error){

}

void DeviceAdapterListenerImpl::OnDeviceDisconnecteDone(const DeviceAdapter* device_adapter, DeviceHandle device_handle){

}
void DeviceAdapterListenerImpl::OnDeviceDisconnecteFailed(const DeviceAdapter* device_adapter, DeviceHandle device_handle, const DisconnectDeviceError& error){

}

void DeviceAdapterListenerImpl::OnDataReceiveDone(const DeviceAdapter* device_adapter, DeviceHandle device_handle, const DataContainerSptr data_container){

}

void DeviceAdapterListenerImpl::OnDataReceiveFailed(const DeviceAdapter* device_adapter, DeviceHandle device_handle, const DataReceiveError& error){

}

void DeviceAdapterListenerImpl::OnDataSendDone(const DeviceAdapter* device_adapter, DeviceHandle device_handle, const DataContainerSptr data_container){

}

void DeviceAdapterListenerImpl::OnDataSendFailed(const DeviceAdapter* device_adapter, DeviceHandle device_handle, const DataSendError& error){

}

void DeviceAdapterListenerImpl::OnCommunicationError(const DeviceAdapter* device_adapter, CommunicationError& error){

}

}//namespace
