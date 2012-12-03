#ifndef PROTOCOLHANDLER_CLASS
#define PROTOCOLHANDLER_CLASS

#include <map>
#include "Logger.hpp"
#include "Utils/MessageQueue.h"
#include "ProtocolHandler/AppLinkRawMessage.h"
#include "ProtocolHandler/ProtocolPacket.h"
#include "TransportManager/ITransportManagerDataListener.hpp"

namespace NsProtocolHandler
{
    class IProtocolObserver;
    class ISessionObserver;  

    class ProtocolHandler : public NsAppLink::NsTransportManager::ITransportManagerDataListener
    {
    public:
        ProtocolHandler( NsAppLink::NsTransportManager::ITransportManager * transportManager );
        ~ProtocolHandler();

        void setProtocolObserver( IProtocolObserver * observer );
        void setSessionObserver( ISessionObserver * observer );

        void sendEndSessionAck( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned int sessionID, 
              unsigned char protocolVersion,
              unsigned int hashCode = 0,
              unsigned char serviceType = SERVICE_TYPE_RPC);
        void sendEndSessionNAck( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned int sessionID,
              unsigned char serviceType = SERVICE_TYPE_RPC );
        void sendStartSessionAck( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned char sessionID,
              unsigned char protocolVersion,
              unsigned int hashCode = 0,
              unsigned char serviceType = SERVICE_TYPE_RPC );
        void sendStartSessionNAck( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned char sessionID,
              unsigned char serviceType = SERVICE_TYPE_RPC );

        void sendData(const AppLinkRawMessage * message);
    
    protected:
        static void * handleMessagesFromMobileApp( void * params );
        static void * handleMessagesToMobileApp( void * params );

    private:
        struct IncomingMessage
        {
          NsAppLink::NsTransportManager::tConnectionHandle mConnectionHandle;
          unsigned char * mData;
          unsigned int mDataSize;
        };

        /**
         * @brief Processing frame received callbacks.
         *
         * @param ConnectionHandle Connection handle.
         * @param Data Received frame payload data.
         * @param DataSize Size of data in bytes.
        **/
        virtual void onFrameReceived(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);
        
        RESULT_CODE sendSingleFrameMessage(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
                                      const unsigned char sessionID,
                                      unsigned int protocolVersion,
                                      const unsigned char servType,
                                      const unsigned int dataSize,
                                      const unsigned char *data,
                                      const bool compress);
        RESULT_CODE sendMultiFrameMessage(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
                                         const unsigned char sessionID,
                                         unsigned int protocolVersion,
                                         const unsigned char servType,
                                         const unsigned int dataSize,
                                         const unsigned char *data,
                                         const bool compress,
                                         const unsigned int maxDataSize);
        
        RESULT_CODE sendFrame( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              const ProtocolPacket & packet );
        RESULT_CODE handleMessage( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              ProtocolPacket * packet );
        RESULT_CODE handleMultiFrameMessage( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              ProtocolPacket * packet );
        RESULT_CODE handleControlMessage( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              const ProtocolPacket * packet );
        

        static log4cplus::Logger mLogger;

        IProtocolObserver * mProtocolObserver;
        ISessionObserver * mSessionObserver;
        NsAppLink::NsTransportManager::ITransportManager * mTransportManager;

        MessageQueue<IncomingMessage *> mMessagesFromMobileApp;
        pthread_t mHandleMessagesFromMobileApp;
        MessageQueue<const AppLinkRawMessage *> mMessagesToMobileApp;
        pthread_t mHandleMessagesToMobileApp;

        std::map<int, ProtocolPacket *> mIncompleteMultiFrameMessages;
        std::map<unsigned char, unsigned int> mMessageCounters;
    };
}

#endif  //  PROTOCOLHANDLER_CLASS