#include "IDeviceAdapterListener.hpp"

AppLink::TransportManager::IDeviceAdapterListener::~IDeviceAdapterListener(void )
{
}

void AppLink::TransportManager::IDeviceAdapterListener::onDeviceListUpdated(AppLink::TransportManager::IDeviceAdapter * DeviceAdapter, const std::vector<AppLink::TransportManager::SInternalDeviceInfo> & DeviceList)
{
}

void AppLink::TransportManager::IDeviceAdapterListener::onDeviceConnected(AppLink::TransportManager::IDeviceAdapter * DeviceAdapter, const AppLink::TransportManager::SInternalDeviceInfo & ConnectedDevice)
{
}

void AppLink::TransportManager::IDeviceAdapterListener::onDeviceDisconnected(AppLink::TransportManager::IDeviceAdapter * DeviceAdapter, const AppLink::TransportManager::SInternalDeviceInfo & DisconnectedDevice)
{
}

void AppLink::TransportManager::IDeviceAdapterListener::onFrameReceived(AppLink::TransportManager::IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const Blob & Data)
{
}

void AppLink::TransportManager::IDeviceAdapterListener::onFrameSendCompleted(AppLink::TransportManager::IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
