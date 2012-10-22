#include "TransportManager/ITransportManagerListener.hpp"

AppLink::TransportManager::ITransportManagerListener::~ITransportManagerListener(void)
{
}

void AppLink::TransportManager::ITransportManagerListener::onDeviceListUpdated(const std::vector<SDeviceInfo> & DeviceList)
{
}

void AppLink::TransportManager::ITransportManagerListener::onDeviceConnected(const SDeviceInfo & ConnectedDevice)
{
}

void AppLink::TransportManager::ITransportManagerListener::onDeviceDisconnected(const SDeviceInfo & DisconnectedDevice)
{
}

void AppLink::TransportManager::ITransportManagerListener::onFrameReceived(tConnectionHandle ConnectionHandle, const Blob & Data)
{
}

void AppLink::TransportManager::ITransportManagerListener::onFrameSendCompleted(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
