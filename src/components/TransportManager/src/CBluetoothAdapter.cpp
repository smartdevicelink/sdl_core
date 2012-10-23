#include "CBluetoothAdapter.hpp"

NsAppLink::NsTransportManager::CBluetoothAdapter::CBluetoothAdapter(NsAppLink::NsTransportManager::IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator):
mListener(Listener),
mHandleGenerator(HandleGenerator),
mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("BluetoothAdapter")))
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter constructed");
}

NsAppLink::NsTransportManager::CBluetoothAdapter::~CBluetoothAdapter(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter destructor");
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::run(void)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::getDeviceList(std::vector<NsAppLink::NsTransportManager::SInternalDeviceInfo> & DeviceList) const
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

NsAppLink::NsTransportManager::EDeviceType NsAppLink::NsTransportManager::CBluetoothAdapter::getDeviceType(void) const
{
    return DeviceBluetooth;
}

int NsAppLink::NsTransportManager::CBluetoothAdapter::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const NsAppLink::NsTransportManager::Blob& Data)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return -1;
}
