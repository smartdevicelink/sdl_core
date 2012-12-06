#include "LoggerHelper.hpp"

#include "CTransportManager.hpp"
#include "CBluetoothAdapter.hpp"
#include "CTCPAdapter.hpp"
#include "TransportManagerLoggerHelpers.hpp"

#include <algorithm>

using namespace NsAppLink::NsTransportManager;


//TODO Add shutdown flag checking inside function calls
//TODO Fix potential crash due to not thread-safe access to shutdown flag
//TODO Check all structures for copy constructor and operators implementation
//TODO Move AppConnected/Disconnected callbacks calling to ConnectionThread

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
mLastUsedConnectionHandle(0),
mApplicationCallbacksThread(),
mDeviceListenersConditionVar(),
mDeviceListenersCallbacks(),
mTerminateFlag(false),
mDevicesByAdapter(),
mDevicesByAdapterMutex(),
mConnections(),
mClientInterfaceMutex()
{
    pthread_mutex_init(&mDataListenersMutex, 0);
    pthread_mutex_init(&mDeviceListenersMutex, 0);
    pthread_mutex_init(&mDeviceHandleGenerationMutex, 0);
    pthread_mutex_init(&mConnectionHandleGenerationMutex, 0);
    pthread_mutex_init(&mClientInterfaceMutex, 0);

    pthread_cond_init(&mDeviceListenersConditionVar, NULL);

    LOG4CPLUS_INFO_EXT(mLogger, "TransportManager constructed");
}

NsAppLink::NsTransportManager::CTransportManager::~CTransportManager(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "TransportManager destructor");

    mTerminateFlag = true;

    // Terminating all threads
    stopApplicationCallbacksThread();
    LOG4CPLUS_INFO_EXT(mLogger, "Waiting for application callbacks thread termination");
    pthread_join(mApplicationCallbacksThread, 0);
    LOG4CPLUS_INFO_EXT(mLogger, "Application callbacks thread terminated");

    pthread_mutex_lock(&mDataListenersMutex);
    std::map<tConnectionHandle, pthread_t> dataThreads;
    for (tConnectionsMap::iterator ConnectionsIterator = mConnections.begin(); ConnectionsIterator != mConnections.end(); ++ConnectionsIterator)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionsIterator->first, "Stopping connection");
        dataThreads[ConnectionsIterator->first] = ConnectionsIterator->second->mConnectionThread;
        stopConnection(ConnectionsIterator->second->mConnectionHandle);
    }
    pthread_mutex_unlock(&mDataListenersMutex);

    for (std::map<tConnectionHandle, pthread_t>::iterator threadsIterator = dataThreads.begin(); threadsIterator != dataThreads.end(); ++threadsIterator)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, threadsIterator->first, "Waiting for thread stoping");
        pthread_join(threadsIterator->second, 0);
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, threadsIterator->first, "Thread terminated");
    }

    LOG4CPLUS_INFO_EXT(mLogger, "All data callbacks threads terminated. Terminating device adapters");

    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        removeDeviceAdapter((*di));
        delete (*di);
    }

    LOG4CPLUS_INFO_EXT(mLogger, "All device adapters removed");

    pthread_mutex_destroy(&mDataListenersMutex);
    pthread_mutex_destroy(&mDeviceListenersMutex);
    pthread_mutex_destroy(&mDeviceHandleGenerationMutex);
    pthread_mutex_destroy(&mConnectionHandleGenerationMutex);
    pthread_mutex_destroy(&mClientInterfaceMutex);

    pthread_cond_destroy(&mDeviceListenersConditionVar);

    LOG4CPLUS_INFO_EXT(mLogger, "Component terminated");
}

void NsAppLink::NsTransportManager::CTransportManager::run(void)
{
    pthread_mutex_lock(&mClientInterfaceMutex);
    initializeDeviceAdapters();

    LOG4CPLUS_INFO_EXT(mLogger, "Starting device adapters");
    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        (*di)->run();
    }

    startApplicationCallbacksThread();
    pthread_mutex_unlock(&mClientInterfaceMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::scanForNewDevices(void)
{
    pthread_mutex_lock(&mClientInterfaceMutex);
    LOG4CPLUS_INFO_EXT(mLogger, "Scanning new devices on all registered device adapters");
    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Initiating scanning of new devices on adapter: " <<(*di)->getDeviceType());
        (*di)->scanForNewDevices();
        LOG4CPLUS_INFO_EXT(mLogger, "Scanning of new devices initiated on adapter: " <<(*di)->getDeviceType());
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Scanning of new devices initiated");
    pthread_mutex_unlock(&mClientInterfaceMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::connectDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    pthread_mutex_lock(&mClientInterfaceMutex);
    connectDisconnectDevice(DeviceHandle, true);
    pthread_mutex_unlock(&mClientInterfaceMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::disconnectDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    pthread_mutex_lock(&mClientInterfaceMutex);
    connectDisconnectDevice(DeviceHandle, false);
    pthread_mutex_unlock(&mClientInterfaceMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::addDataListener(NsAppLink::NsTransportManager::ITransportManagerDataListener * Listener)
{
    pthread_mutex_lock(&mClientInterfaceMutex);
    mDataListeners.push_back(Listener);
    pthread_mutex_unlock(&mClientInterfaceMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::removeDataListener(NsAppLink::NsTransportManager::ITransportManagerDataListener * Listener)
{
    pthread_mutex_lock(&mClientInterfaceMutex);
    mDataListeners.erase(std::remove(mDataListeners.begin(), mDataListeners.end(), Listener), mDataListeners.end());
    pthread_mutex_unlock(&mClientInterfaceMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::addDeviceListener(NsAppLink::NsTransportManager::ITransportManagerDeviceListener * Listener)
{
    pthread_mutex_lock(&mClientInterfaceMutex);
    mDeviceListeners.push_back(Listener);
    pthread_mutex_unlock(&mClientInterfaceMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::removeDeviceListener(NsAppLink::NsTransportManager::ITransportManagerDeviceListener * Listener)
{
    pthread_mutex_lock(&mClientInterfaceMutex);
    mDeviceListeners.erase(std::remove(mDeviceListeners.begin(), mDeviceListeners.end(), Listener), mDeviceListeners.end());
    pthread_mutex_unlock(&mClientInterfaceMutex);
}

void NsAppLink::NsTransportManager::CTransportManager::sendFrame(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize, const int UserData)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "sendFrame called. DataSize: "<<DataSize);

    // TODO Add incomming parameters checking

    // Searching device adapter
    pthread_mutex_lock(&mDataListenersMutex);
    SConnectionInfo* connectionInfo = getConnection(ConnectionHandle);
    pthread_mutex_unlock(&mDataListenersMutex);

    if(0 != connectionInfo)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Device adapter found (type: "<<connectionInfo->mpDeviceAdapter.getDeviceType()<<"). Sending frame to it");
        connectionInfo->mpDeviceAdapter.sendFrame(ConnectionHandle, Data, DataSize, UserData);
    }
    else
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Device adapter that handles Connection Handle was not found");
    }
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

void CTransportManager::onDeviceListUpdated(IDeviceAdapter * DeviceAdapter, const tInternalDeviceList & DeviceList)
{
    if(0 == DeviceAdapter)
    {
        LOG4CPLUS_WARN_EXT(mLogger, "DeviceAdapter=0");
    }
    else
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Device adapter type is: "<<DeviceAdapter->getDeviceType() << ", number of devices is: "<<DeviceList.size());
        pthread_mutex_lock(&mDevicesByAdapterMutex);

        tDevicesByAdapterMap::iterator devicesIterator = mDevicesByAdapter.find(DeviceAdapter);
        if(devicesIterator == mDevicesByAdapter.end())
        {
            LOG4CPLUS_WARN_EXT(mLogger, "Invalid adapter initialization. No devices vector available for adapter: "<<DeviceAdapter->getDeviceType());
            pthread_mutex_unlock(&mDevicesByAdapterMutex);
        }
        else
        {
            // Updating devices for adapter
            tInternalDeviceList *pDevices = devicesIterator->second;
            pDevices->clear();
            std::copy(DeviceList.begin(), DeviceList.end(), std::back_inserter(*pDevices));

            LOG4CPLUS_INFO_EXT(mLogger, "Devices list for adapter is updated. Adapter type is: "<<DeviceAdapter->getDeviceType());

            pthread_mutex_unlock(&mDevicesByAdapterMutex);

            // Calling callback with new device list to subscribers
            sendDeviceListUpdatedCallback();
        }
    }
}

void CTransportManager::onApplicationConnected(IDeviceAdapter * DeviceAdapter, const SDeviceInfo & ConnectedDevice, const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "onApplicationConnected");

    if(0 == DeviceAdapter)
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "ApplicationConnected received from invalid device adapter");
        return;
    }

    if(InvalidConnectionHandle == ConnectionHandle)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "ApplicationConnected received with invalid connection handle");
        return;
    }

    if(InvalidDeviceHandle == ConnectedDevice.mDeviceHandle)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "ApplicationConnected received with invalid device handle: "<<ConnectedDevice.mDeviceHandle);
        return;
    }

    if(DeviceAdapter->getDeviceType() != ConnectedDevice.mDeviceType)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "ApplicationConnected received but connected device type("<<ConnectedDevice.mDeviceType<<") differs from device adapters type: "<<DeviceAdapter->getDeviceType());
        return;
    }

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Before mDevicesByAdapterMutex mutex lock");
    
    pthread_mutex_lock(&mDevicesByAdapterMutex);
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Right after mDevicesByAdapterMutex mutex lock");
    tDevicesByAdapterMap::iterator devicesIterator = mDevicesByAdapter.find(DeviceAdapter);
    if(devicesIterator == mDevicesByAdapter.end())
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Invalid device adapter initialization. No devices vector available for adapter: "<<DeviceAdapter->getDeviceType());
        pthread_mutex_unlock(&mDevicesByAdapterMutex);
        return;
    }

    tInternalDeviceList *pDevices = devicesIterator->second;
    bool connectionHandleFound = false;
    for(tInternalDeviceList::const_iterator deviceIterator = pDevices->begin(); deviceIterator != pDevices->end(); ++deviceIterator)
    {
        if(deviceIterator->mDeviceHandle == ConnectedDevice.mDeviceHandle)
        {
            connectionHandleFound = true;
        }
    }

    pthread_mutex_unlock(&mDevicesByAdapterMutex);

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "After mDevicesByAdapterMutex mutex unlock");

    if(false == connectionHandleFound)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Connected device handle ("<<ConnectedDevice.mDeviceHandle<<") was not found in devices list for adapter of type: "<<DeviceAdapter->getDeviceType());
        return;
    }

    startConnection(ConnectionHandle, *DeviceAdapter);

    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Sending callback");

    // Sending callback
    SDeviceListenerCallback cb(CTransportManager::DeviceListenerCallbackType_ApplicationConnected, ConnectedDevice, ConnectionHandle);
    sendDeviceCallback(cb);

    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "END of onApplicationConnected");
}

void CTransportManager::onApplicationDisconnected(IDeviceAdapter* DeviceAdapter, const SDeviceInfo& DisconnectedDevice, const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "onApplicationDisconnected");

    if(0 == DeviceAdapter)
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "ApplicationDisconnected received from invalid device adapter");
        return;
    }

    if(InvalidConnectionHandle == ConnectionHandle)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "ApplicationDisconnected received with invalid connection handle");
        return;
    }

    if(InvalidDeviceHandle == DisconnectedDevice.mDeviceHandle)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "ApplicationDisconnected received with invalid device handle: "<<DisconnectedDevice.mDeviceHandle);
        return;
    }

    if(DeviceAdapter->getDeviceType() != DisconnectedDevice.mDeviceType)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "ApplicationDisconnected received but disconnected device type("<<DisconnectedDevice.mDeviceType<<") differs from device adapters type: "<<DeviceAdapter->getDeviceType());
        return;
    }

    pthread_mutex_lock(&mDevicesByAdapterMutex);
    tDevicesByAdapterMap::iterator devicesIterator = mDevicesByAdapter.find(DeviceAdapter);
    if(devicesIterator == mDevicesByAdapter.end())
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Invalid device adapter initialization. No devices vector available for adapter: "<<DeviceAdapter->getDeviceType());
        pthread_mutex_unlock(&mDevicesByAdapterMutex);
        return;
    }

    tInternalDeviceList *pDevices = devicesIterator->second;
    bool connectionHandleFound = false;
    for(tInternalDeviceList::const_iterator deviceIterator = pDevices->begin(); deviceIterator != pDevices->end(); ++deviceIterator)
    {
        if(deviceIterator->mDeviceHandle == DisconnectedDevice.mDeviceHandle)
        {
            connectionHandleFound = true;
        }
    }

    pthread_mutex_unlock(&mDevicesByAdapterMutex);

    if(false == connectionHandleFound)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Disconnected device handle ("<<DisconnectedDevice.mDeviceHandle<<") was not found in devices list for adapter of type: "<<DeviceAdapter->getDeviceType());
        return;
    }

    pthread_mutex_lock(&mDataListenersMutex);
    SConnectionInfo* pConnection = getConnection(ConnectionHandle);
    pthread_mutex_unlock(&mDataListenersMutex);

    if(0 == pConnection)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Thread for connection does not exist");
        return;
    }

    pthread_mutex_lock(&mDataListenersMutex);
    if(true == pConnection->mTerminateFlag)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Connection is already in shutdown state.");
        pthread_mutex_unlock(&mDataListenersMutex);
    }
    else
    {
        stopConnection(ConnectionHandle);
        pthread_mutex_unlock(&mDataListenersMutex);

        // Sending callback
        SDeviceListenerCallback cb(CTransportManager::DeviceListenerCallbackType_ApplicationDisconnected, DisconnectedDevice, ConnectionHandle);
        sendDeviceCallback(cb);
    }

    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "END of onApplicationDisconnected");
}

void CTransportManager::onFrameReceived(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "onFrameReceived called. DA: "<<DeviceAdapter<<", DataSize: "<<DataSize);

    if(0 == DeviceAdapter)
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "onFrameReceived received from invalid device adapter");
        return;
    }

    if(0 == Data)
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "onFrameReceived with empty data");
        return;
    }

    if(0 == DataSize)
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "onFrameReceived with DataSize=0");
        return;
    }

    if(InvalidConnectionHandle == ConnectionHandle)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "onFrameReceived received with invalid connection handle");
        return;
    }

    pthread_mutex_lock(&mDevicesByAdapterMutex);
    tDevicesByAdapterMap::iterator devicesIterator = mDevicesByAdapter.find(DeviceAdapter);
    if(devicesIterator == mDevicesByAdapter.end())
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "onFrameReceived. Invalid device adapter initialization. No devices vector available for adapter: "<<DeviceAdapter->getDeviceType());
        pthread_mutex_unlock(&mDevicesByAdapterMutex);
        return;
    }
    pthread_mutex_unlock(&mDevicesByAdapterMutex);

    pthread_mutex_lock(&mDataListenersMutex);
    SConnectionInfo* connectionInfo = getConnection(ConnectionHandle);
    pthread_mutex_unlock(&mDataListenersMutex);

    if(0 == connectionInfo)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "onFrameReceived. Connection information for connection does not exist");
        return;
    }

    //TODO: Currently all frames processed in one thread. In the future processing of them
    //      must be moved to the thread, which sent callbacks for corresponded connection

    pthread_mutex_lock(&mDataListenersMutex);
    connectionInfo->mFrameData.appendFrameData(Data, DataSize);

    uint8_t *pFramePacketData = 0;
    size_t FramePacketSize = 0;

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Starting frame extraction");
    while(true == connectionInfo->mFrameData.extractFrame(pFramePacketData, FramePacketSize))
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Frame extracted. Size is: "<< FramePacketSize);
        SDataListenerCallback newCallback(CTransportManager::DataListenerCallbackType_FrameReceived, ConnectionHandle, pFramePacketData, FramePacketSize);
        sendDataCallback(newCallback);
        delete pFramePacketData;
        FramePacketSize = 0;
    }

    pthread_mutex_unlock(&mDataListenersMutex);

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "onFrameReceived processed");
}

void CTransportManager::onFrameSendCompleted(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "onFrameSendCompleted called. DA: "<<DeviceAdapter<<", UserData: "<<UserData <<", SendStatus: " <<SendStatus);

    if(0 == DeviceAdapter)
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "onFrameSendCompleted received from invalid device adapter");
        return;
    }

    if(InvalidConnectionHandle == ConnectionHandle)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "onFrameSendCompleted received with invalid connection handle");
        return;
    }

    pthread_mutex_lock(&mDevicesByAdapterMutex);
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "searching for devices for adapter");
    tDevicesByAdapterMap::iterator devicesIterator = mDevicesByAdapter.find(DeviceAdapter);
    if(devicesIterator == mDevicesByAdapter.end())
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "onFrameSendCompleted. Invalid device adapter initialization. No devices vector available for adapter: "<<DeviceAdapter->getDeviceType());
        pthread_mutex_unlock(&mDevicesByAdapterMutex);
        return;
    }
    pthread_mutex_unlock(&mDevicesByAdapterMutex);

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Checking connection information availability");
    
    pthread_mutex_lock(&mDataListenersMutex);
    bool bThreadExist = isConnectionAvailable(ConnectionHandle);
    pthread_mutex_unlock(&mDataListenersMutex);

    if(false == bThreadExist)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "onFrameSendCompleted. Connection is not available");
        return;
    }

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Connection is available. Preparing callback");

    SDataListenerCallback newCallback(CTransportManager::DataListenerCallbackType_FrameSendCompleted, ConnectionHandle, UserData, SendStatus);

    pthread_mutex_lock(&mDataListenersMutex);
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Sending callback");
    sendDataCallback(newCallback);
    pthread_mutex_unlock(&mDataListenersMutex);
}

CTransportManager::SDeviceListenerCallback::SDeviceListenerCallback(CTransportManager::EDeviceListenerCallbackType CallbackType, const tDeviceList& DeviceList)
: mCallbackType(CallbackType)
, mDeviceList(DeviceList)
, mDeviceInfo()
, mConnectionHandle()
{
}

CTransportManager::SDeviceListenerCallback::SDeviceListenerCallback(CTransportManager::EDeviceListenerCallbackType CallbackType, const SDeviceInfo& DeviceInfo, const tConnectionHandle& ConnectionHandle)
: mCallbackType(CallbackType)
, mDeviceList()
, mDeviceInfo(DeviceInfo)
, mConnectionHandle(ConnectionHandle)
{
}

CTransportManager::SDeviceListenerCallback::SDeviceListenerCallback(const SDeviceListenerCallback& other)
: mCallbackType(other.mCallbackType)
, mDeviceList(other.mDeviceList)
, mDeviceInfo(other.mDeviceInfo)
, mConnectionHandle(other.mConnectionHandle)
{
}

bool CTransportManager::SDeviceListenerCallback::operator==( const SDeviceListenerCallback& i_other ) const
{
    return ( (mCallbackType == i_other.mCallbackType)
          && (mDeviceList == i_other.mDeviceList)
          && (mDeviceInfo == i_other.mDeviceInfo)
          && (mConnectionHandle == i_other.mConnectionHandle));
}

CTransportManager::SDeviceListenerCallback::~SDeviceListenerCallback(void )
{
}

CTransportManager::SDataListenerCallback::SDataListenerCallback(CTransportManager::EDataListenerCallbackType CallbackType, tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize)
: mCallbackType(CallbackType)
, mConnectionHandle(ConnectionHandle)
, mData(0)
, mDataSize(DataSize)
, mUserData()
, mSendStatus(NsAppLink::NsTransportManager::SendStatusUnknownError)
{
    if ((0 != Data) &&
        (0u != DataSize))
    {
        mData = new uint8_t[DataSize];

        if (0 != mData)
        {
            memcpy(mData, Data, DataSize);     
        }
    }
}

CTransportManager::SDataListenerCallback::SDataListenerCallback(CTransportManager::EDataListenerCallbackType CallbackType, tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus)
: mCallbackType(CallbackType)
, mConnectionHandle(ConnectionHandle)
, mData(0)
, mDataSize(0)
, mUserData(UserData)
, mSendStatus(SendStatus)
{

}

NsAppLink::NsTransportManager::CTransportManager::SDataListenerCallback::SDataListenerCallback(const SDataListenerCallback& other)
: mCallbackType(other.mCallbackType)
, mConnectionHandle(other.mConnectionHandle)
, mData(0)
, mDataSize(other.mDataSize)
, mUserData(other.mUserData)
, mSendStatus(other.mSendStatus)
{
    if ((0 != other.mData) &&
        (0u != other.mDataSize))
    {
        mData = new uint8_t[other.mDataSize];

        if (0 != mData)
        {
            mDataSize = other.mDataSize;
            memcpy(mData, other.mData, mDataSize);
        }
    }
}

bool NsAppLink::NsTransportManager::CTransportManager::SDataListenerCallback::operator==( const SDataListenerCallback& i_other ) const
{
    return ( (mCallbackType == i_other.mCallbackType)
          && (mConnectionHandle == i_other.mConnectionHandle)
          && (mDataSize == i_other.mDataSize)
          && (mUserData == i_other.mUserData)
          && (mSendStatus == i_other.mSendStatus)
          && (0 == memcmp(mData, i_other.mData, i_other.mDataSize)));
}

CTransportManager::SDataListenerCallback::~SDataListenerCallback(void )
{
    if (0 != mData)
    {
        delete[] mData;
    }
}

CTransportManager::SDataThreadStartupParams::SDataThreadStartupParams(CTransportManager* TransportManager, tConnectionHandle ConnectionHandle)
: mTransportManager(TransportManager)
, mConnectionHandle(ConnectionHandle)
{
}


void CTransportManager::applicationCallbacksThread()
{
    LOG4CPLUS_INFO_EXT(mLogger, "Started application callbacks thread");

    while(false == mTerminateFlag)
    {
        pthread_mutex_lock(&mDeviceListenersMutex);

        while(mDeviceListenersCallbacks.empty() && (false == mTerminateFlag))
        {
            LOG4CPLUS_INFO_EXT(mLogger, "No callbacks to process. Waiting");
            pthread_cond_wait(&mDeviceListenersConditionVar, &mDeviceListenersMutex);
            LOG4CPLUS_INFO_EXT(mLogger, "Callbacks processing triggered");
        }

        if(mTerminateFlag)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Shutdown is on progress. Skipping callback processing.");
            pthread_mutex_unlock(&mDeviceListenersMutex);
            break;
        }

        LOG4CPLUS_INFO_EXT(mLogger, "Copying callbacks and device listeners to process");

        std::vector<SDeviceListenerCallback> callbacksToProcess(mDeviceListenersCallbacks);
        mDeviceListenersCallbacks.clear();

        std::vector<ITransportManagerDeviceListener*> deviceListenersToSend(mDeviceListeners);

        pthread_mutex_unlock(&mDeviceListenersMutex);

        LOG4CPLUS_INFO_EXT(mLogger, "Starting callbacks processing. Number of callbacks: " << callbacksToProcess.size());

        std::vector<SDeviceListenerCallback>::const_iterator callbackIterator;

        for(callbackIterator = callbacksToProcess.begin(); callbackIterator != callbacksToProcess.end(); ++callbackIterator)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Processing callback of type: " << (*callbackIterator).mCallbackType);

            std::vector<ITransportManagerDeviceListener*>::const_iterator deviceListenersIterator;
            int deviceListenerIndex = 0;

            for (deviceListenersIterator = deviceListenersToSend.begin(), deviceListenerIndex=0; deviceListenersIterator != deviceListenersToSend.end(); ++deviceListenersIterator, ++deviceListenerIndex)
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Calling callback on listener #" << deviceListenerIndex);

                switch((*callbackIterator).mCallbackType)
                {
                    case CTransportManager::DeviceListenerCallbackType_DeviceListUpdated:
                        (*deviceListenersIterator)->onDeviceListUpdated((*callbackIterator).mDeviceList);
                        break;
                    case CTransportManager::DeviceListenerCallbackType_ApplicationConnected:
                        (*deviceListenersIterator)->onApplicationConnected((*callbackIterator).mDeviceInfo, (*callbackIterator).mConnectionHandle);
                        break;
                    case CTransportManager::DeviceListenerCallbackType_ApplicationDisconnected:
                        (*deviceListenersIterator)->onApplicationDisconnected((*callbackIterator).mDeviceInfo, (*callbackIterator).mConnectionHandle);
                        break;
                    default:
                        LOG4CPLUS_ERROR_EXT(mLogger, "Unknown callback type: " << (*callbackIterator).mCallbackType);
                        break;
                }

                LOG4CPLUS_INFO_EXT(mLogger, "Callback on listener #" << deviceListenerIndex <<" called");
            }
        }

        LOG4CPLUS_INFO_EXT(mLogger, "All callbacks processed. Starting next callbacks processing iteration");
    }

    LOG4CPLUS_INFO_EXT(mLogger, "ApplicationsCallback thread terminated");
}

void* CTransportManager::applicationCallbacksThreadStartRoutine(void* Data)
{
    if (0 != Data)
    {
        static_cast<CTransportManager*>(Data)->applicationCallbacksThread();
    }

    return 0;
}

void CTransportManager::dataCallbacksThread(const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Started data callbacks thread");

    pthread_mutex_lock(&mDataListenersMutex);
    SConnectionInfo* connectionInfo = getConnection(ConnectionHandle);
    pthread_mutex_unlock(&mDataListenersMutex);

    if(0 == connectionInfo)
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "Connection information was not found");
        return;
    }
    
    while(false == connectionInfo->mTerminateFlag)
    {
        pthread_mutex_lock(&mDataListenersMutex);

        while(connectionInfo->mDataCallbacksVector.empty() && (false == connectionInfo->mTerminateFlag))
        {
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "No callbacks to process. Waiting");
            pthread_cond_wait(&connectionInfo->mConditionVar, &mDataListenersMutex);
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Callbacks processing triggered");
        }

        if(connectionInfo->mTerminateFlag)
        {
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Shutdown is on progress. Skipping callback processing.");
            pthread_mutex_unlock(&mDataListenersMutex);
            break;
        }

        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Copying callbacks and device listeners to process");

        tDataCallbacksVector callbacksToProcess(connectionInfo->mDataCallbacksVector);
        connectionInfo->mDataCallbacksVector.clear();

        std::vector<ITransportManagerDataListener*> dataListenersToSend(mDataListeners);

        pthread_mutex_unlock(&mDataListenersMutex);

        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Starting callbacks processing. Number of callbacks: " << callbacksToProcess.size());

        tDataCallbacksVector::const_iterator callbackIterator;
        for(callbackIterator = callbacksToProcess.begin(); callbackIterator != callbacksToProcess.end(); ++callbackIterator)
        {
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Processing callback of type: " << (*callbackIterator).mCallbackType);

            if(ConnectionHandle != callbackIterator->mConnectionHandle)
            {
                TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "Possible error. Thread connection handle ("<<ConnectionHandle<<") differs from callback connection handle ("<<callbackIterator->mConnectionHandle<<")");
            }

            std::vector<ITransportManagerDataListener*>::const_iterator dataListenersIterator;
            int dataListenerIndex = 0;

            for (dataListenersIterator = dataListenersToSend.begin(), dataListenerIndex=0; dataListenersIterator != dataListenersToSend.end(); ++dataListenersIterator, ++dataListenerIndex)
            {
                TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Calling callback on listener #" << dataListenerIndex);

                switch((*callbackIterator).mCallbackType)
                {
                    case CTransportManager::DataListenerCallbackType_FrameReceived:
                        (*dataListenersIterator)->onFrameReceived(callbackIterator->mConnectionHandle, callbackIterator->mData, callbackIterator->mDataSize);
                        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Callback onFrameReceived on listener #" << dataListenerIndex << " was called. DataSize: " << callbackIterator->mDataSize);
                        break;
                    case CTransportManager::DataListenerCallbackType_FrameSendCompleted:
                        (*dataListenersIterator)->onFrameSendCompleted(callbackIterator->mConnectionHandle, callbackIterator->mUserData, callbackIterator->mSendStatus);
                        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Callback onFrameReceived on listener #" << dataListenerIndex << " was called. UserData: " << callbackIterator->mUserData<<", SendStatus: "<<callbackIterator->mSendStatus);
                        break;
                    default:
                        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "Unknown callback type: " << (*callbackIterator).mCallbackType);
                        break;
                }

                LOG4CPLUS_INFO_EXT(mLogger, "Callback on listener #" << dataListenerIndex <<" called"<<", ConnectionHandle: "<<ConnectionHandle);
            }
        }

        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "All callbacks processed. Starting next callbacks processing iteration");
    }

    pthread_mutex_lock(&mDataListenersMutex);

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Terminating connection thread");

    // Deleting data associated with connection handle
    delete connectionInfo;
    mConnections.erase(ConnectionHandle);
    pthread_mutex_unlock(&mDataListenersMutex);
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Connection thread terminated");
}

void* CTransportManager::dataCallbacksThreadStartRoutine(void* Data)
{
    SDataThreadStartupParams * parameters = static_cast<SDataThreadStartupParams*>(Data);

    if (0 != parameters)
    {
        CTransportManager *pTransportManager = parameters->mTransportManager;
        tConnectionHandle connectionHandle(parameters->mConnectionHandle);

        delete parameters;
        parameters = 0;

        pTransportManager->dataCallbacksThread(connectionHandle);
    }

    return 0;
}

void CTransportManager::startApplicationCallbacksThread()
{
    LOG4CPLUS_INFO_EXT(mLogger, "Starting device listeners thread");

    int errorCode = pthread_create(&mApplicationCallbacksThread, 0, &applicationCallbacksThreadStartRoutine, this);

    if (0 == errorCode)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Device listeners thread started");
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Device listeners thread cannot be started, error code " << errorCode);
    }
}

void CTransportManager::stopApplicationCallbacksThread()
{
    LOG4CPLUS_TRACE_EXT(mLogger, "Stopping application-callbacks thread");

    pthread_mutex_lock(&mDeviceListenersMutex);
    pthread_cond_signal(&mDeviceListenersConditionVar);
    pthread_mutex_unlock(&mDeviceListenersMutex);
}

void CTransportManager::startConnection(const tConnectionHandle ConnectionHandle, IDeviceAdapter & DeviceAdapter)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Starting data-callbacks thread");

    pthread_mutex_lock(&mDataListenersMutex);

    if(isConnectionAvailable(ConnectionHandle))
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "Connection with this handle already available. Possible error.");
    }
    else
    {
        SConnectionInfo *connection = new SConnectionInfo(ConnectionHandle, DeviceAdapter);
        SDataThreadStartupParams *connectionThreadParams = new SDataThreadStartupParams(this, ConnectionHandle);

        int errorCode = pthread_create(&connection->mConnectionThread, 0, &dataCallbacksThreadStartRoutine, connectionThreadParams);

        if (0 == errorCode)
        {
            mConnections[ConnectionHandle] = connection;
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Connection started.");
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Thread start for connection handle (" << ConnectionHandle << ") failed, error code " << errorCode);
            delete connection;
            delete connectionThreadParams;
        }
    }

    pthread_mutex_unlock(&mDataListenersMutex);
}

void CTransportManager::stopConnection(const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Stopping data-callbacks thread");

    SConnectionInfo* connectionInfo = getConnection(ConnectionHandle);
    if(0 == connectionInfo)
    {
        //pthread_mutex_unlock(&mDataListenersMutex);
        return;
    }
    else
    {
        connectionInfo->mTerminateFlag = true;
        pthread_cond_signal(&connectionInfo->mConditionVar);
    }
}


bool CTransportManager::isConnectionAvailable(const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Checking connection availability");

    bool bConnectionExist = (mConnections.find(ConnectionHandle) != mConnections.end());

    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Result of checking is: " << bConnectionExist);

    return bConnectionExist;
}

CTransportManager::SConnectionInfo* CTransportManager::getConnection(const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Checking connection availability");

    if(ConnectionHandle == InvalidConnectionHandle)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Trying to get connection for invalid handle.");
        return 0;
    }

    tConnectionsMap::iterator ConnectionIterator = mConnections.find(ConnectionHandle);
    if(ConnectionIterator == mConnections.end())
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Connection info was not found");
        return 0;
    }

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Connection info was found");
    return ConnectionIterator->second;
}


void CTransportManager::addDeviceAdapter(IDeviceAdapter* DeviceAdapter)
{
    mDeviceAdapters.push_back(DeviceAdapter);
    mDevicesByAdapter.insert(std::make_pair(DeviceAdapter, new tInternalDeviceList()));
}

void CTransportManager::removeDeviceAdapter(IDeviceAdapter* DeviceAdapter)
{
    tDevicesByAdapterMap::iterator devicesIterator = mDevicesByAdapter.find(DeviceAdapter);
    if(devicesIterator != mDevicesByAdapter.end())
    {
        delete devicesIterator->second;
        mDevicesByAdapter.erase(DeviceAdapter);
    }
}

void CTransportManager::initializeDeviceAdapters()
{
    addDeviceAdapter(new CBluetoothAdapter(*this, *this));
    addDeviceAdapter(new CTCPAdapter(*this, *this));
    LOG4CPLUS_INFO_EXT(mLogger, "Device adapters initialized");
}

void CTransportManager::sendDeviceListUpdatedCallback()
{
    LOG4CPLUS_INFO_EXT(mLogger, "Preparing complete device list from all adapters");

    // Preparing complete device list
    tDeviceList devices;

    tDevicesByAdapterMap::const_iterator deviceAdaptersIterator;

    pthread_mutex_lock(&mDevicesByAdapterMutex);

    for(deviceAdaptersIterator = mDevicesByAdapter.begin(); deviceAdaptersIterator != mDevicesByAdapter.end(); ++deviceAdaptersIterator)
    {
        IDeviceAdapter* pDeviceAdapter = deviceAdaptersIterator->first;
        tInternalDeviceList *pDevices = deviceAdaptersIterator->second;

        LOG4CPLUS_INFO_EXT(mLogger, "Processing adapter with type: "<<pDeviceAdapter->getDeviceType());

        tInternalDeviceList::const_iterator devicesInAdapterIterator;
        for(devicesInAdapterIterator = pDevices->begin(); devicesInAdapterIterator != pDevices->end(); ++devicesInAdapterIterator)
        {
            SDeviceInfo deviceInfo;

            deviceInfo.mDeviceHandle = devicesInAdapterIterator->mDeviceHandle;
            deviceInfo.mDeviceType = pDeviceAdapter->getDeviceType();
            deviceInfo.mUniqueDeviceId = devicesInAdapterIterator->mUniqueDeviceId;
            deviceInfo.mUserFriendlyName = devicesInAdapterIterator->mUserFriendlyName;

            devices.push_back(deviceInfo);

            LOG4CPLUS_INFO_EXT(mLogger, "Processed device with unique Id: "<<devicesInAdapterIterator->mUniqueDeviceId << ", friendlyName: "<<devicesInAdapterIterator->mUserFriendlyName);
        }
    }

    LOG4CPLUS_INFO_EXT(mLogger, "Complete device list from all adapters was prepared. Preparing callback OnDeviceListUpdated for sending");

    pthread_mutex_unlock(&mDevicesByAdapterMutex);

    // Sending DeviceListUpdatedCallback
    SDeviceListenerCallback cb(CTransportManager::DeviceListenerCallbackType_DeviceListUpdated, devices);
    sendDeviceCallback(cb);

    LOG4CPLUS_INFO_EXT(mLogger, "Callback OnDeviceListUpdated was prepared for sending");
}

void CTransportManager::connectDisconnectDevice(const tDeviceHandle DeviceHandle, bool Connect)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Performing "<<(Connect?"CONNECT":"DISCONNECT")<<" for device with handle: " << DeviceHandle);
    LOG4CPLUS_INFO_EXT(mLogger, "Searching for device adapter for handling DeviceHandle: " << DeviceHandle);

    tDevicesByAdapterMap::const_iterator deviceAdaptersIterator;

    pthread_mutex_lock(&mDevicesByAdapterMutex);

    IDeviceAdapter* pDeviceAdapterToCall = 0;

    for(deviceAdaptersIterator = mDevicesByAdapter.begin(); deviceAdaptersIterator != mDevicesByAdapter.end(); ++deviceAdaptersIterator)
    {
        IDeviceAdapter* pDeviceAdapter = deviceAdaptersIterator->first;
        tInternalDeviceList *pDevices = deviceAdaptersIterator->second;

        LOG4CPLUS_INFO_EXT(mLogger, "Processing adapter with type: "<<pDeviceAdapter->getDeviceType());

        tInternalDeviceList::const_iterator devicesInAdapterIterator;
        for(devicesInAdapterIterator = pDevices->begin(); devicesInAdapterIterator != pDevices->end(); ++devicesInAdapterIterator)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Processing device with unique Id: "<<devicesInAdapterIterator->mUniqueDeviceId << ", DeviceHandle: "<<devicesInAdapterIterator->mDeviceHandle);
            if(devicesInAdapterIterator->mDeviceHandle == DeviceHandle)
            {
                LOG4CPLUS_INFO_EXT(mLogger, "DeviceHandle relates to adapter: "<<pDeviceAdapter->getDeviceType());
                pDeviceAdapterToCall = pDeviceAdapter;
            }
        }
    }

    pthread_mutex_unlock(&mDevicesByAdapterMutex);

    if(0 != pDeviceAdapterToCall)
    {
        if(Connect)
        {
            pDeviceAdapterToCall->connectDevice(DeviceHandle);
        }
        else
        {
            pDeviceAdapterToCall->disconnectDevice(DeviceHandle);
        }
        LOG4CPLUS_INFO_EXT(mLogger, (Connect?"CONNECT":"DISCONNECT")<<" operation performed on device with handle: " << DeviceHandle);
    }
    else
    {
        LOG4CPLUS_WARN_EXT(mLogger, (Connect?"CONNECT":"DISCONNECT")<<" operation was not performed. Device handle was not found on any device: " << DeviceHandle);
    }
}

void CTransportManager::sendDataCallback(const CTransportManager::SDataListenerCallback& callback)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, callback.mConnectionHandle, "Preparing callback of type: "<<callback.mCallbackType<<", to send");

    SConnectionInfo* connectionInfo = getConnection(callback.mConnectionHandle);
    if(0 == connectionInfo)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, callback.mConnectionHandle, "Connection information was not found");
    }
    else
    {
        connectionInfo->mDataCallbacksVector.push_back(callback);
        pthread_cond_signal(&connectionInfo->mConditionVar);
    }
}

void CTransportManager::sendDeviceCallback(const CTransportManager::SDeviceListenerCallback& callback)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Sending new device callback. Type: " << callback.mCallbackType);
    pthread_mutex_lock(&mDeviceListenersMutex);

    mDeviceListenersCallbacks.push_back(callback);
    LOG4CPLUS_INFO_EXT(mLogger, "Device callback was added to pool");

    pthread_cond_signal(&mDeviceListenersConditionVar);

    pthread_mutex_unlock(&mDeviceListenersMutex);
    LOG4CPLUS_INFO_EXT(mLogger, "Triggering device callback processing");
}

CTransportManager::SFrameDataForConnection::SFrameDataForConnection(tConnectionHandle ConnectionHandle)
: mDataSize(0)
, mConnectionHandle(ConnectionHandle)
, mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManager")))
{
    mBufferSize = 1536;
    mpDataBuffer = new uint8_t[mBufferSize];

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Initialized frame data for connection container");
}

CTransportManager::SFrameDataForConnection::~SFrameDataForConnection()
{
    if(0 != mpDataBuffer)
    {
        delete mpDataBuffer;
        mpDataBuffer = 0;
    }
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, mConnectionHandle, "Frame data for connection container destroyed");
}

void CTransportManager::SFrameDataForConnection::appendFrameData(const uint8_t* Data, size_t DataSize)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, mConnectionHandle, "Appending data to container. DataSize: " << DataSize);

    // Checking that data can be added to existing buffer
    if( (mDataSize+DataSize) <= mBufferSize)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, mConnectionHandle, "Data can be appended to existing buffer. Buffer size: "<<mBufferSize<<", Existing data size: "<<mDataSize<<", DataSize: " << DataSize);
        memcpy(&mpDataBuffer[mDataSize], Data, DataSize);
        mDataSize += DataSize;
    }
    else
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, mConnectionHandle, "Data cannot be appended to existing buffer. Buffer size: "<<mBufferSize<<", Existing data size: "<<mDataSize<<", DataSize: " << DataSize);

        size_t newSize = mBufferSize + DataSize; //TODO Think about more correct buffer allocation
        uint8_t *newBuffer = new uint8_t[newSize];

        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, mConnectionHandle, "New buffer allocated. Buffer size: "<<newSize<<", was: "<<mBufferSize);

        memcpy(newBuffer, mpDataBuffer, mDataSize);
        memcpy(&newBuffer[mDataSize], Data, DataSize);
        delete mpDataBuffer;
        mpDataBuffer = 0; // Paranoid mode on

        mpDataBuffer = newBuffer;
        mDataSize = mDataSize+DataSize;
        mBufferSize = newSize;
    }

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, mConnectionHandle, "Data appended. Buffer size: "<<mBufferSize<<", Existing data size: "<<mDataSize);
}

bool CTransportManager::SFrameDataForConnection::extractFrame(uint8_t *& Data, size_t & DataSize)
{
    if(mDataSize < PROTOCOL_HEADER_V1_SIZE)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, mConnectionHandle, "Not enough data for version in the buffer. No changes was made. mDataSize: "<<mDataSize);
        return false;
    }

    // Extracting version
    uint8_t firstByte = mpDataBuffer[0];
    uint8_t version = firstByte >> 4;

    uint8_t offset = sizeof(uint32_t);
    uint32_t frameDataSize  = mpDataBuffer[offset++] << 24;
    frameDataSize  |= mpDataBuffer[offset++] << 16;
    frameDataSize  |= mpDataBuffer[offset++] << 8;
    frameDataSize  |= mpDataBuffer[offset++];

    size_t requiredDataSize = 0;

    if(version == PROTOCOL_VERSION_1)
    {
        requiredDataSize = (PROTOCOL_HEADER_V1_SIZE + frameDataSize);
    }
    else if(version == PROTOCOL_VERSION_2)
    {
        requiredDataSize = (PROTOCOL_HEADER_V2_SIZE + frameDataSize);
    }
    else
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, mConnectionHandle, "Unsupported version received: " << version);
        return false;
        //TODO: Think about what to do in that case. Possible solution - signal about that and terminate connection
        // For that method CDeviceAdapter::StopConnection can be used (must be unprotected before)
    }

    if(mDataSize < requiredDataSize)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, mConnectionHandle, "Frame canot be extracted. Its size: " << requiredDataSize << ", Available data size: "<<mDataSize);
        return false;
    }

    // Extracting frame from buffer
    Data = new uint8_t[requiredDataSize];
    memcpy(Data, mpDataBuffer, requiredDataSize);
    DataSize = requiredDataSize;

    // Shifting buffer
    mDataSize -= requiredDataSize;
    memmove(mpDataBuffer, &mpDataBuffer[requiredDataSize], mDataSize);

    return true;
}


NsAppLink::NsTransportManager::CTransportManager::SFrameDataForConnection::SFrameDataForConnection(const SFrameDataForConnection& other)
: mDataSize(other.mDataSize)
, mBufferSize(other.mBufferSize)
, mConnectionHandle(other.mConnectionHandle)
, mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManager")))
{
    mpDataBuffer = new uint8_t[other.mBufferSize];
    memcpy(mpDataBuffer, other.mpDataBuffer, other.mBufferSize);

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, mConnectionHandle, "Initialized frame data for connection container");
}

bool NsAppLink::NsTransportManager::CTransportManager::SFrameDataForConnection::operator==( const SFrameDataForConnection& i_other ) const
{
    return ( (mDataSize == i_other.mDataSize)
          && (mBufferSize == i_other.mBufferSize)
          && (mConnectionHandle == i_other.mConnectionHandle)
          && (0 == memcmp(mpDataBuffer, i_other.mpDataBuffer, mBufferSize))
          );
}

CTransportManager::SConnectionInfo::SConnectionInfo(const tConnectionHandle ConnectionHandle, IDeviceAdapter & DeviceAdapter)
: mConnectionHandle(ConnectionHandle)
, mTerminateFlag(false)
, mDataCallbacksVector()
, mConnectionThread()
, mConditionVar()
, mpDeviceAdapter(DeviceAdapter)
, mFrameData(ConnectionHandle)
{
    pthread_cond_init(&mConditionVar, NULL);
}

CTransportManager::SConnectionInfo::SConnectionInfo(const CTransportManager::SConnectionInfo& other)
: mConnectionHandle(other.mConnectionHandle)
, mTerminateFlag(other.mTerminateFlag)
, mDataCallbacksVector(other.mDataCallbacksVector)
, mConnectionThread(other.mConnectionThread)
, mConditionVar(other.mConditionVar)
, mpDeviceAdapter(other.mpDeviceAdapter)
, mFrameData(other.mFrameData)
{

}

bool CTransportManager::SConnectionInfo::operator==(const CTransportManager::SConnectionInfo& i_other) const
{
    return ( (mConnectionHandle == i_other.mConnectionHandle)
//           && (mTerminateFlag == i_other.mTerminateFlag)
//           && (mDataCallbacksVector == i_other.mDataCallbacksVector)
//           && (mConnectionThread == i_other.mConnectionThread)
//           && (mConditionVar == i_other.mConditionVar)
//           && (mpDeviceAdapter == i_other.mpDeviceAdapter)
//           && (mFrameData == i_other.mFrameData)
           );
}

CTransportManager::SConnectionInfo::~SConnectionInfo()
{
    pthread_cond_destroy(&mConditionVar);
}
