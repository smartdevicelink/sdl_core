#include "LoggerHelper.hpp"

#include "CTransportManager.hpp"
#include "CBluetoothAdapter.hpp"

#include <algorithm>

NsAppLink::NsTransportManager::CTransportManager::CTransportManager(void):
mDeviceAdapters(),
mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManager"))),
mDataListenersMutex(),
mDeviceListenersMutex(),
mDeviceHandleGenerationMutex(),
mConnectionHandleGenerationMutex(),
mDataListeners(),
mDeviceListeners(),
mLastUsedDeviceHandle(0),
mLastUsedConnectionHandle(0)
{
    mDeviceAdapters.push_back(new CBluetoothAdapter(*this, *this));

    pthread_mutex_init(&mDataListenersMutex, 0);
    pthread_mutex_init(&mDeviceListenersMutex, 0);
    pthread_mutex_init(&mDeviceHandleGenerationMutex, 0);
    pthread_mutex_init(&mConnectionHandleGenerationMutex, 0);

    LOG4CPLUS_INFO_EXT(mLogger, "TransportManager constructed");
}

NsAppLink::NsTransportManager::CTransportManager::~CTransportManager(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "TransportManager destructor");

    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        delete *di;
    }

    pthread_mutex_destroy(&mDataListenersMutex);
    pthread_mutex_destroy(&mDeviceListenersMutex);
    pthread_mutex_destroy(&mDeviceHandleGenerationMutex);
    pthread_mutex_destroy(&mConnectionHandleGenerationMutex);
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

void NsAppLink::NsTransportManager::CTransportManager::addDataListener(NsAppLink::NsTransportManager::ITransportManagerDataListener * Listener)
{
    pthread_mutex_lock(&mDataListenersMutex);
    mDataListeners.push_back(Listener);
    pthread_mutex_unlock(&mDataListenersMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::removeDataListener(NsAppLink::NsTransportManager::ITransportManagerDataListener * Listener)
{
    pthread_mutex_lock(&mDataListenersMutex);
    mDataListeners.erase(std::remove(mDataListeners.begin(), mDataListeners.end(), Listener), mDataListeners.end());
    pthread_mutex_unlock(&mDataListenersMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::addDeviceListener(NsAppLink::NsTransportManager::ITransportManagerDeviceListener * Listener)
{
    pthread_mutex_lock(&mDeviceListenersMutex);
    mDeviceListeners.push_back(Listener);
    pthread_mutex_unlock(&mDeviceListenersMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::removeDeviceListener(NsAppLink::NsTransportManager::ITransportManagerDeviceListener * Listener)
{
    pthread_mutex_lock(&mDeviceListenersMutex);
    mDeviceListeners.erase(std::remove(mDeviceListeners.begin(), mDeviceListeners.end(), Listener), mDeviceListeners.end());
    pthread_mutex_unlock(&mDeviceListenersMutex);
}

int NsAppLink::NsTransportManager::CTransportManager::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const NsAppLink::NsTransportManager::Blob & Data)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return -1;
}

NsAppLink::NsTransportManager::tDeviceHandle NsAppLink::NsTransportManager::CTransportManager::generateNewDeviceHandle(void)
{
    tDeviceHandle outputDeviceHandle;

    pthread_mutex_lock(&mDeviceHandleGenerationMutex);
    ++mLastUsedDeviceHandle;
    outputDeviceHandle = mLastUsedDeviceHandle;
    pthread_mutex_unlock(&mDeviceHandleGenerationMutex);

    return outputDeviceHandle;
}

NsAppLink::NsTransportManager::tConnectionHandle NsAppLink::NsTransportManager::CTransportManager::generateNewConnectionHandle(void)
{
    tConnectionHandle outputConnectionHandle;

    pthread_mutex_lock(&mConnectionHandleGenerationMutex);
    ++mLastUsedConnectionHandle;
    outputConnectionHandle = mLastUsedConnectionHandle;
    pthread_mutex_unlock(&mConnectionHandleGenerationMutex);

    return outputConnectionHandle;
}
