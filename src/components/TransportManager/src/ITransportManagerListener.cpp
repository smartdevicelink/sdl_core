#include "TransportManager/ITransportManagerListener.hpp"

NsAppLink::NsTransportManager::ITransportManagerListener::~ITransportManagerListener(void)
{
}

void NsAppLink::NsTransportManager::ITransportManagerListener::onDeviceListUpdated(const tDeviceList & DeviceList)
{
}

void NsAppLink::NsTransportManager::ITransportManagerListener::onDeviceConnected(const SDeviceInfo & ConnectedDevice)
{
}

void NsAppLink::NsTransportManager::ITransportManagerListener::onDeviceDisconnected(const SDeviceInfo & DisconnectedDevice)
{
}

void NsAppLink::NsTransportManager::ITransportManagerListener::onFrameReceived(tConnectionHandle ConnectionHandle, const Blob & Data)
{
}

void NsAppLink::NsTransportManager::ITransportManagerListener::onFrameSendCompleted(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
