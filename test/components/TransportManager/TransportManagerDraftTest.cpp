#include "Logger.hpp"
#include "TransportManager/ITransportManager.hpp"
#include "TransportManager/ITransportManagerDataListener.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"

using namespace NsAppLink::NsTransportManager;

// Log helpers for logs related to Connection Handle
#define TM_CH_LOG4CPLUS_INFO_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_INFO_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_INFO_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_INFO_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE_METHOD_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE_METHOD_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_DEBUG_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_DEBUG_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_DEBUG_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_DEBUG_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_WARN_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_WARN_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_WARN_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_WARN_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_ERROR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_ERROR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_ERROR_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_ERROR_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_FATAL_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_FATAL_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_FATAL_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_FATAL_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)


class TransportManagerDraftTest : public ITransportManagerDataListener, public ITransportManagerDeviceListener
{
public:
    TransportManagerDraftTest(NsAppLink::NsTransportManager::ITransportManager *manager)
    : ITransportManagerDataListener()
    , ITransportManagerDeviceListener()
    , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("appMain")))
    , mpTransportManager(manager)
    {
        
    }
    
    virtual void onFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "onFrameReceived. DataSize: "<<DataSize);

        uint8_t buff[50] = {1};
        mpTransportManager->sendFrame(ConnectionHandle, buff, 50);
    }
    
    virtual void onFrameSendCompleted(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, ConnectionHandle, "onFrameSendCompleted called. FrameSequenceNumber: "<<FrameSequenceNumber <<", SendStatus: " <<SendStatus);

        sleep(1);
        uint8_t buff[500] = {1};
        mpTransportManager->sendFrame(ConnectionHandle, buff, 500);
    }

    virtual void onDeviceListUpdated(const tDeviceList & DeviceList)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "onDeviceListUpdated. Number of devices is: "<<DeviceList.size());

        tDeviceList::const_iterator device;
        for(device = DeviceList.begin(); device != DeviceList.end(); ++device)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "   Adapter: "<<device->mDeviceType<<". Handle: "<<device->mDeviceHandle<< ". Unique Id: "<<device->mUniqueDeviceId << ", friendlyName: "<<device->mUserFriendlyName);
            
            mpTransportManager->connectDevice(device->mDeviceHandle);
        }
    }

    virtual void onApplicationConnected(const SDeviceInfo & ConnectedDevice, const tConnectionHandle Connection)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, Connection, "onApplicationConnected.    Adapter: "<<ConnectedDevice.mDeviceType<<". Handle: "<<ConnectedDevice.mDeviceHandle<< ". Unique Id: "<<ConnectedDevice.mUniqueDeviceId << ", friendlyName: "<<ConnectedDevice.mUserFriendlyName);
    }

    virtual void onApplicationDisconnected(const SDeviceInfo & DisconnectedDevice, const tConnectionHandle Connection)
    {
        TM_CH_LOG4CPLUS_INFO_EXT(mLogger, Connection, "onApplicationDisconnected.    Adapter: "<<DisconnectedDevice.mDeviceType<<". Handle: "<<DisconnectedDevice.mDeviceHandle<< ". Unique Id: "<<DisconnectedDevice.mUniqueDeviceId << ", friendlyName: "<<DisconnectedDevice.mUserFriendlyName);
    }

    const log4cplus::Logger mLogger;

    NsAppLink::NsTransportManager::ITransportManager *mpTransportManager;
};


int main(int argc, char** argv)
{
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("appMain"));
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    LOG4CPLUS_INFO(logger, " Application started!");

    NsAppLink::NsTransportManager::ITransportManager * transportManager = NsAppLink::NsTransportManager::ITransportManager::create();

    // Test TM client
    TransportManagerDraftTest *pTest = new TransportManagerDraftTest(transportManager);

    transportManager->addDataListener(pTest);
    transportManager->addDeviceListener(pTest);
    
    transportManager->run();
    transportManager->scanForNewDevices();

    while(true)
    {
        sleep(1);
    }

    return EXIT_SUCCESS;
} 