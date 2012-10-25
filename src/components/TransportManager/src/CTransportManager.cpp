#include "LoggerHelper.hpp"

#include "CTransportManager.hpp"
#include "CBluetoothAdapter.hpp"

NsAppLink::NsTransportManager::CTransportManager::CTransportManager(void):
mDeviceAdapters(),
mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManager")))
{
    mDeviceAdapters.push_back(new CBluetoothAdapter(*this, *this));

    LOG4CPLUS_INFO_EXT(mLogger, "TransportManager constructed");
}

NsAppLink::NsTransportManager::CTransportManager::~CTransportManager(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "TransportManager destructor");

    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        delete *di;
    }
}

void NsAppLink::NsTransportManager::CTransportManager::run(void)
{
    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        (*di)->run();
    }
}

void NsAppLink::NsTransportManager::CTransportManager::getDeviceList(NsAppLink::NsTransportManager::tDeviceList & DeviceList) const
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

void NsAppLink::NsTransportManager::CTransportManager::addListener(NsAppLink::NsTransportManager::ITransportManagerListener * Listener)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

void NsAppLink::NsTransportManager::CTransportManager::removeListener(NsAppLink::NsTransportManager::ITransportManagerListener * Listener)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

int NsAppLink::NsTransportManager::CTransportManager::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const NsAppLink::NsTransportManager::Blob & Data)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return -1;
}

NsAppLink::NsTransportManager::tDeviceHandle NsAppLink::NsTransportManager::CTransportManager::generateNewDeviceHandle(void)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return InvalidDeviceHandle;
}

NsAppLink::NsTransportManager::tConnectionHandle NsAppLink::NsTransportManager::CTransportManager::generateNewConnectionHandle(void)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return InvalidConnectionHandle;
}
