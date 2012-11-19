#include "TransportManager/ITransportManagerDeviceListener.hpp"

NsAppLink::NsTransportManager::ITransportManagerDeviceListener::~ITransportManagerDeviceListener()
{

}

void NsAppLink::NsTransportManager::ITransportManagerDeviceListener::onDeviceListUpdated(const tDeviceList & DeviceList)
{
}

void NsAppLink::NsTransportManager::ITransportManagerDeviceListener::onApplicationConnected(const SDeviceInfo & ConnectedDevice, const tConnectionHandle Connection)
{
}

void NsAppLink::NsTransportManager::ITransportManagerDeviceListener::onApplicationDisconnected(const SDeviceInfo & DisconnectedDevice, const tConnectionHandle Connection)
{
}