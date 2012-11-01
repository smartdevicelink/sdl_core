#include "TransportManager/ITransportManagerDeviceListener.hpp"

NsAppLink::NsTransportManager::ITransportManagerDeviceListener::~ITransportManagerDeviceListener()
{

}

void NsAppLink::NsTransportManager::ITransportManagerDeviceListener::onDeviceListUpdated(const tDeviceList & DeviceList)
{
}

void NsAppLink::NsTransportManager::ITransportManagerDeviceListener::onDeviceConnected(const SDeviceInfo & ConnectedDevice)
{
}

void NsAppLink::NsTransportManager::ITransportManagerDeviceListener::onDeviceDisconnected(const SDeviceInfo & DisconnectedDevice)
{
}