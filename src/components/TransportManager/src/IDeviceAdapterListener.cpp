/**
 * \file IDeviceAdapterListener.cpp
 * \brief Class IDeviceAdapterListener.
 * Copyright (c) 2013 Ford Motor Company
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
