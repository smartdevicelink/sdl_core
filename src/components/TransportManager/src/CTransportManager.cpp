#include "LoggerHelper.hpp"

#include "CTransportManager.hpp"
#include "CBluetoothAdapter.hpp"

AppLink::TransportManager::CTransportManager::CTransportManager(void):
mDeviceAdapters(),
mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManager")))
{
    mDeviceAdapters.push_back(new CBluetoothAdapter(*this, *this));

    LOG4CPLUS_INFO_EXT(mLogger, "TransportManager constructed");
}

AppLink::TransportManager::CTransportManager::~CTransportManager(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "TransportManager destructor");

    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        delete *di;
    }
}

void AppLink::TransportManager::CTransportManager::run(void)
{
    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        (*di)->run();
    }
}

void AppLink::TransportManager::CTransportManager::getDeviceList(std::vector<AppLink::TransportManager::SDeviceInfo> & DeviceList) const
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

void AppLink::TransportManager::CTransportManager::addListener(AppLink::TransportManager::ITransportManagerListener * Listener)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

void AppLink::TransportManager::CTransportManager::removeListener(AppLink::TransportManager::ITransportManagerListener * Listener)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

int AppLink::TransportManager::CTransportManager::sendFrame(AppLink::TransportManager::tConnectionHandle ConnectionHandle, const AppLink::TransportManager::Blob & Data)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return -1;
}

AppLink::TransportManager::tDeviceHandle AppLink::TransportManager::CTransportManager::generateNewDeviceHandle(void)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return InvalidDeviceHandle;
}

AppLink::TransportManager::tConnectionHandle AppLink::TransportManager::CTransportManager::generateNewConnectionHandle(void)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return InvalidConnectionHandle;
}
