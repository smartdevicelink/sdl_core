/**
 * \file ITransportManagerDeviceListener.cpp
 * \brief Class ITransportManagerDeviceListener.
 * Copyright (c) 2013 Ford Motor Company
 */

#include "TransportManager/ITransportManagerDeviceListener.hpp"

NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener::~ITransportManagerDeviceListener()
{

}

void NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener::onDeviceListUpdated(const tDeviceList & DeviceList)
{
}

void NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener::onApplicationConnected(const SDeviceInfo & ConnectedDevice, const tConnectionHandle Connection)
{
}

void NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener::onApplicationDisconnected(const SDeviceInfo & DisconnectedDevice, const tConnectionHandle Connection)
{
}
