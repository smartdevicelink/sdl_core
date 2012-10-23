#include "CBluetoothAdapter.hpp"

AppLink::TransportManager::CBluetoothAdapter::CBluetoothAdapter(AppLink::TransportManager::IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator):
mListener(Listener),
mHandleGenerator(HandleGenerator),
mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("BluetoothAdapter")))
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter constructed");
}

AppLink::TransportManager::CBluetoothAdapter::~CBluetoothAdapter(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter destructor");
}

void AppLink::TransportManager::CBluetoothAdapter::run(void)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

void AppLink::TransportManager::CBluetoothAdapter::getDeviceList(std::vector<AppLink::TransportManager::SInternalDeviceInfo> & DeviceList) const
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

AppLink::TransportManager::EDeviceType AppLink::TransportManager::CBluetoothAdapter::getDeviceType(void) const
{
    return DeviceBluetooth;
}

int AppLink::TransportManager::CBluetoothAdapter::sendFrame(AppLink::TransportManager::tConnectionHandle ConnectionHandle, const AppLink::TransportManager::Blob& Data)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return -1;
}
