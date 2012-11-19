#include "IDeviceAdapterListener.hpp"

NsAppLink::NsTransportManager::IDeviceAdapterListener::~IDeviceAdapterListener(void )
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onDeviceListUpdated(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsAppLink::NsTransportManager::tInternalDeviceList & DeviceList)
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onApplicationConnected(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsAppLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onApplicationDisconnected(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsAppLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onFrameReceived(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onFrameSendCompleted(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
