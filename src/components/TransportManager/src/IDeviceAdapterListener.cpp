#include "IDeviceAdapterListener.hpp"

NsAppLink::NsTransportManager::IDeviceAdapterListener::~IDeviceAdapterListener(void )
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onDeviceListUpdated(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsAppLink::NsTransportManager::tInternalDeviceList & DeviceList)
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onDeviceConnected(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsAppLink::NsTransportManager::SInternalDeviceInfo & ConnectedDevice)
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onDeviceDisconnected(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, const NsAppLink::NsTransportManager::SInternalDeviceInfo & DisconnectedDevice)
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onFrameReceived(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
}

void NsAppLink::NsTransportManager::IDeviceAdapterListener::onFrameSendCompleted(NsAppLink::NsTransportManager::IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
