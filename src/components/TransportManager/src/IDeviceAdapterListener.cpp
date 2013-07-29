/**
 * \file IDeviceAdapterListener.cpp
 * \brief Class IDeviceAdapterListener.
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

#include "IDeviceAdapterListener.hpp"

NsSmartDeviceLink::NsTransportManager::IDeviceAdapterListener::~IDeviceAdapterListener(void )
{
}

void NsSmartDeviceLink::NsTransportManager::IDeviceAdapterListener::onDeviceListUpdated(NsSmartDeviceLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsSmartDeviceLink::NsTransportManager::tInternalDeviceList & DeviceList)
{
}

void NsSmartDeviceLink::NsTransportManager::IDeviceAdapterListener::onApplicationConnected(NsSmartDeviceLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
}

void NsSmartDeviceLink::NsTransportManager::IDeviceAdapterListener::onApplicationDisconnected(NsSmartDeviceLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
}

void NsSmartDeviceLink::NsTransportManager::IDeviceAdapterListener::onFrameReceived(NsSmartDeviceLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
}

void NsSmartDeviceLink::NsTransportManager::IDeviceAdapterListener::onFrameSendCompleted(NsSmartDeviceLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
