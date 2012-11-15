#include "LoggerHelper.hpp"

#include "CTransportManager.hpp"
#include "CBluetoothAdapter.hpp"
#include "CTCPAdapter.hpp"
#include "TransportManagerLoggerHelpers.hpp"

#include <algorithm>

using namespace NsAppLink::NsTransportManager;




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
mDataListenersCallbacks(),
mDataCallbacksThreads(),
mDataCallbacksConditionVars(),
mDevicesByAdapter(),
mDevicesByAdapterMutex(),
mDeviceAdaptersByConnectionHandle(),
mDeviceAdaptersByConnectionHandleMutex()
{
    pthread_mutex_init(&mDataListenersMutex, 0);
    pthread_mutex_init(&mDeviceListenersMutex, 0);
    pthread_mutex_init(&mDeviceHandleGenerationMutex, 0);
    pthread_mutex_init(&mConnectionHandleGenerationMutex, 0);
    pthread_mutex_init(&mDeviceAdaptersByConnectionHandleMutex, 0);

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

    tDataCallbacksThreads::iterator threadsIterator;
    for (threadsIterator = mDataCallbacksThreads.begin(); threadsIterator != mDataCallbacksThreads.end(); ++threadsIterator)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, threadsIterator->first, "Waiting for thread stoping");
        stopDataCallbacksThread(threadsIterator->first);
        pthread_join(threadsIterator->second, 0);
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, threadsIterator->first, "Thread terminated");
    }

    LOG4CPLUS_INFO_EXT(mLogger, "All data callbacks threads terminated. Terminating device adapters");

    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        removeDeviceAdapter((*di));
    }

    LOG4CPLUS_INFO_EXT(mLogger, "All device adapters removed");

    pthread_mutex_destroy(&mDataListenersMutex);
    pthread_mutex_destroy(&mDeviceListenersMutex);
    pthread_mutex_destroy(&mDeviceHandleGenerationMutex);
    pthread_mutex_destroy(&mConnectionHandleGenerationMutex);
    pthread_mutex_destroy(&mDeviceAdaptersByConnectionHandleMutex);

    pthread_cond_destroy(&mDeviceListenersConditionVar);

    LOG4CPLUS_INFO_EXT(mLogger, "Component terminated");
}

void NsAppLink::NsTransportManager::CTransportManager::run(void)
{
    initializeDeviceAdapters();

    LOG4CPLUS_INFO_EXT(mLogger, "Starting device adapters");
    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        (*di)->run();
        //(*di)->scanForNewDevices();
    }

    if( false == startApplicationCallbacksThread())
    {
        return;
    }
}

void NsAppLink::NsTransportManager::CTransportManager::scanForNewDevices(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Scanning new devices on all registered device adapters");
    for (std::vector<IDeviceAdapter*>::iterator di = mDeviceAdapters.begin(); di != mDeviceAdapters.end(); ++di)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Initiating scanning of new devices on adapter: " <<(*di)->getDeviceType());
        (*di)->scanForNewDevices();
        LOG4CPLUS_INFO_EXT(mLogger, "Scanning of new devices initiated on adapter: " <<(*di)->getDeviceType());
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Scanning of new devices initiated");
}

void NsAppLink::NsTransportManager::CTransportManager::connectDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    connectDisconnectDevice(DeviceHandle, true);
}

void NsAppLink::NsTransportManager::CTransportManager::disconnectDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    connectDisconnectDevice(DeviceHandle, false);
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

int NsAppLink::NsTransportManager::CTransportManager::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "sendFrame called. DataSize: "<<DataSize);

    int result = -1;

    // Searching device adapter
    pthread_mutex_lock(&mDeviceAdaptersByConnectionHandleMutex);

    tDeviceAdaptersByConnectionHandleMap::iterator deviceAdapterIterator = mDeviceAdaptersByConnectionHandle.find(ConnectionHandle);
    if(deviceAdapterIterator != mDeviceAdaptersByConnectionHandle.end())
    {
        IDeviceAdapter *pDeviceAdapter = deviceAdapterIterator->second;
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Device adapter found (type: "<<pDeviceAdapter->getDeviceType()<<"). Sending frame to it");
        result = pDeviceAdapter->sendFrame(ConnectionHandle, Data, DataSize);
    }
    else
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Device adapter that handles Connection Handle was not found");
    }

    pthread_mutex_unlock(&mDeviceAdaptersByConnectionHandleMutex);

    return result;
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

    startDataCallbacksThread(ConnectionHandle);

    // Storing device adapter for that handle
    pthread_mutex_lock(&mDeviceAdaptersByConnectionHandleMutex);
    mDeviceAdaptersByConnectionHandle.insert(std::make_pair(ConnectionHandle, DeviceAdapter));
    pthread_mutex_unlock(&mDeviceAdaptersByConnectionHandleMutex);

    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Sending callback");
    
    // Sending callback
    SDeviceListenerCallback cb(CTransportManager::DeviceListenerCallbackType_ApplicationConnected, ConnectedDevice, ConnectionHandle);
    sendDeviceCallback(cb);

    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "END of onApplicationConnected");
}

void CTransportManager::onApplicationDisconnected(IDeviceAdapter* DeviceAdapter, const SDeviceInfo& DisconnectedDevice, const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "onApplicationDisconnected");

    stopDataCallbacksThread(ConnectionHandle);

    // Removing device adapter for that handle
    pthread_mutex_lock(&mDeviceAdaptersByConnectionHandleMutex);
    mDeviceAdaptersByConnectionHandle.erase(ConnectionHandle);
    pthread_mutex_unlock(&mDeviceAdaptersByConnectionHandleMutex);

    // Sending callback
    SDeviceListenerCallback cb(CTransportManager::DeviceListenerCallbackType_ApplicationDisconnected, DisconnectedDevice, ConnectionHandle);
    sendDeviceCallback(cb);

    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "END of onApplicationDisconnected");
}

void CTransportManager::onFrameReceived(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "onFrameReceived called. DA: "<<DeviceAdapter<<", DataSize: "<<DataSize);
    SDataListenerCallback newCallback(CTransportManager::DataListenerCallbackType_FrameReceived, ConnectionHandle, Data, DataSize);
    sendDataCallback(newCallback);
}

void CTransportManager::onFrameSendCompleted(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "onFrameSendCompleted called. DA: "<<DeviceAdapter<<", FrameSequenceNumber: "<<FrameSequenceNumber <<", SendStatus: " <<SendStatus);
    SDataListenerCallback newCallback(CTransportManager::DataListenerCallbackType_FrameSendCompleted, ConnectionHandle, FrameSequenceNumber, SendStatus);
    sendDataCallback(newCallback);
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

CTransportManager::SDeviceListenerCallback::~SDeviceListenerCallback(void )
{
}

CTransportManager::SDataListenerCallback::SDataListenerCallback(CTransportManager::EDataListenerCallbackType CallbackType, tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize)
: mCallbackType(CallbackType)
, mConnectionHandle(ConnectionHandle)
, mData(Data)
, mDataSize(DataSize)
, mFrameSequenceNumber(-1)
, mSendStatus(NsAppLink::NsTransportManager::SendStatusUnknownError)
{

}

CTransportManager::SDataListenerCallback::SDataListenerCallback(CTransportManager::EDataListenerCallbackType CallbackType, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
: mCallbackType(CallbackType)
, mConnectionHandle(ConnectionHandle)
, mData(0)
, mDataSize(0)
, mFrameSequenceNumber(FrameSequenceNumber)
, mSendStatus(SendStatus)
{

}

CTransportManager::SDataListenerCallback::~SDataListenerCallback(void )
{

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

    // Checking condition variable
    tDataCallbacksConditionVariables::iterator conditionVarIterator = mDataCallbacksConditionVars.find(ConnectionHandle);
    if(conditionVarIterator == mDataCallbacksConditionVars.end())
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "Condition variable was not found on thread start.");
        pthread_mutex_unlock(&mDataListenersMutex);
        return;
    }
    pthread_cond_t *conditionVar = conditionVarIterator->second;

    // Checking callbacks vector
    tDataCallbacks::iterator callbacksMapIterator = mDataListenersCallbacks.find(ConnectionHandle);
    if(callbacksMapIterator == mDataListenersCallbacks.end())
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "Callbacks vector was not found on thread start.");
        pthread_mutex_unlock(&mDataListenersMutex);
        return;
    }
    tDataCallbacksVector *pCallbacksVector = callbacksMapIterator->second;
    pthread_mutex_unlock(&mDataListenersMutex);

    
    while(false == mTerminateFlag)
    {
        pthread_mutex_lock(&mDataListenersMutex);

        while(pCallbacksVector->empty() && (false == mTerminateFlag))
        {
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "No callbacks to process. Waiting");
            pthread_cond_wait(conditionVar, &mDataListenersMutex);
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Callbacks processing triggered");
        }

        if(mTerminateFlag)
        {
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Shutdown is on progress. Skipping callback processing.");
            pthread_mutex_unlock(&mDataListenersMutex);
            break;
        }

        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Copying callbacks and device listeners to process");

        tDataCallbacksVector callbacksToProcess(*pCallbacksVector);
        pCallbacksVector->clear();

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
                        (*dataListenersIterator)->onFrameSendCompleted(callbackIterator->mConnectionHandle, callbackIterator->mFrameSequenceNumber, callbackIterator->mSendStatus);
                        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Callback onFrameReceived on listener #" << dataListenerIndex << " was called. FrameSequenceNumber: " << callbackIterator->mFrameSequenceNumber<<", SendStatus: "<<callbackIterator->mSendStatus);
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

    tDataCallbacksConditionVariables::iterator conditionVarDeleteIterator = mDataCallbacksConditionVars.find(ConnectionHandle);
    if(conditionVarDeleteIterator != mDataCallbacksConditionVars.end())
    {
        delete conditionVarDeleteIterator->second;
        mDataCallbacksConditionVars.erase(ConnectionHandle);
    }
    else
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Failed to remove condition variable");
    }

    tDataCallbacks::iterator callbacksVectorIterator = mDataListenersCallbacks.find(ConnectionHandle);
    if(callbacksVectorIterator != mDataListenersCallbacks.end())
    {
        callbacksVectorIterator->second->clear();
        delete callbacksVectorIterator->second;
        mDataListenersCallbacks.erase(ConnectionHandle);
    }
    else
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Failed to remove callbacks vector");
    }

    tDataCallbacksThreads::iterator threadIterator = mDataCallbacksThreads.find(ConnectionHandle);
    if(threadIterator != mDataCallbacksThreads.end())
    {
        mDataCallbacksThreads.erase(ConnectionHandle);
    }
    else
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, ConnectionHandle, "Failed to remove thread");
    }

    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Connection thread terminated");
    pthread_mutex_unlock(&mDataListenersMutex);
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

bool CTransportManager::startApplicationCallbacksThread()
{
    LOG4CPLUS_INFO_EXT(mLogger, "Starting device listeners thread");

    int errorCode = pthread_create(&mApplicationCallbacksThread, 0, &applicationCallbacksThreadStartRoutine, this);

    if (0 == errorCode)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Device listeners thread started");
        return true;
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Device listeners thread cannot be started, error code " << errorCode);
        return false;
    }
}

void CTransportManager::stopApplicationCallbacksThread()
{
    LOG4CPLUS_TRACE_EXT(mLogger, "Stopping application-callbacks thread");

    pthread_mutex_lock(&mDeviceListenersMutex);
    pthread_cond_signal(&mDeviceListenersConditionVar);
    pthread_mutex_unlock(&mDeviceListenersMutex);
}

bool CTransportManager::startDataCallbacksThread(const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Starting data-callbacks thread");

    pthread_mutex_lock(&mDataListenersMutex);

    if(isThreadForConnectionHandleExist(ConnectionHandle))
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "Thread already exist. Possible error.");
    }
    else
    {
        pthread_t connectionThread;
        SDataThreadStartupParams *connectionThreadParams = new SDataThreadStartupParams(this, ConnectionHandle);

        int errorCode = pthread_create(&connectionThread, 0, &dataCallbacksThreadStartRoutine, connectionThreadParams);

        if (0 == errorCode)
        {
            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Thread started. Preparing callbacks vector and condition variable");

            mDataCallbacksThreads.insert(std::make_pair(ConnectionHandle, connectionThread));

            pthread_cond_t *pConnectionConditionVariable = new pthread_cond_t();
            pthread_cond_init(pConnectionConditionVariable, NULL);
            mDataCallbacksConditionVars.insert(std::make_pair(ConnectionHandle, pConnectionConditionVariable));

            tDataCallbacksVector *pConnectionCallbacks = new tDataCallbacksVector();
            mDataListenersCallbacks.insert(std::make_pair(ConnectionHandle, pConnectionCallbacks));

            TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "Callbacks vector and condition variable prepared");
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Thread start for connection handle (" << ConnectionHandle << ") failed, error code " << errorCode);
            delete connectionThreadParams;
        }
    }

    pthread_mutex_unlock(&mDataListenersMutex);
}

void CTransportManager::stopDataCallbacksThread(const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Stopping data-callbacks thread");

    pthread_mutex_lock(&mDataListenersMutex);

    tDataCallbacksConditionVariables::iterator conditionVarIterator = mDataCallbacksConditionVars.find(ConnectionHandle);

    if(conditionVarIterator == mDataCallbacksConditionVars.end())
    {
        TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, ConnectionHandle, "ConnectionHandle is invalid");
    }
    else
    {
        pthread_cond_t *conditionVar = conditionVarIterator->second;
        pthread_cond_signal(conditionVar);
    }

    pthread_mutex_unlock(&mDataListenersMutex);
}


bool CTransportManager::isThreadForConnectionHandleExist(const tConnectionHandle ConnectionHandle)
{
    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Checking whether thread for connection handle exist");

    bool bThreadExist = (mDataCallbacksThreads.find(ConnectionHandle) != mDataCallbacksThreads.end());

    TM_CH_LOG4CPLUS_TRACE_EXT(mLogger, ConnectionHandle, "Result of checking is: " << bThreadExist);

    return bThreadExist;
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

                if(Connect)
                {
                    pDeviceAdapter->connectDevice(DeviceHandle);
                }
                else
                {
                    pDeviceAdapter->disconnectDevice(DeviceHandle);
                }
            }
        }
    }

    LOG4CPLUS_INFO_EXT(mLogger, (Connect?"CONNECT":"DISCONNECT")<<" operation performed on device with handle: " << DeviceHandle);

    pthread_mutex_unlock(&mDevicesByAdapterMutex);
}

void CTransportManager::sendDataCallback(const CTransportManager::SDataListenerCallback& callback)
{
    TM_CH_LOG4CPLUS_INFO_EXT(mLogger, callback.mConnectionHandle, "Preparing callback of type: "<<callback.mCallbackType<<", to send");

    pthread_mutex_lock(&mDataListenersMutex);

    tDataCallbacksConditionVariables::iterator conditionVarIterator = mDataCallbacksConditionVars.find(callback.mConnectionHandle);

    if(conditionVarIterator == mDataCallbacksConditionVars.end())
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, callback.mConnectionHandle, "Condition variable was not found. Possible problems with connection initialization or connection handle is invalid");
    }
    else
    {
        pthread_cond_t *conditionVar = conditionVarIterator->second;

        tDataCallbacks::iterator callbacksMapIterator = mDataListenersCallbacks.find(callback.mConnectionHandle);
        if(callbacksMapIterator == mDataListenersCallbacks.end())
        {
            TM_CH_LOG4CPLUS_ERROR_EXT(mLogger, callback.mConnectionHandle, "Callbacks vector was not found. Possible problems with connection initialization or connection handle is invalid");
        }
        else
        {
            tDataCallbacksVector *pCallbacksVector = callbacksMapIterator->second;
            pCallbacksVector->push_back(callback);

            pthread_cond_signal(conditionVar);
        }
    }

    pthread_mutex_unlock(&mDataListenersMutex);
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
    mBufferSize = 512;
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

        size_t newSize = mBufferSize * 3;
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

bool CTransportManager::SFrameDataForConnection::extractFrame(uint8_t * Data, size_t & DataSize)
{
    if(mDataSize < PROTOCOL_HEADER_V1_SIZE)
    {
        TM_CH_LOG4CPLUS_WARN_EXT(mLogger, mConnectionHandle, "Not enough data for version in the buffer. No changes was made");
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
    DataSize = frameDataSize;

    // Shifting buffer
    mDataSize -= requiredDataSize;
    memmove(mpDataBuffer, &mpDataBuffer[requiredDataSize], mDataSize);

    return true;
}
