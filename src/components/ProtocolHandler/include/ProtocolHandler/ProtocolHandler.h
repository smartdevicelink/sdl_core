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

    class ProtocolHandler : public NsAppLink::NsTransportManager::ITransportManagerDataListener
    {
    public:
        ProtocolHandler(IProtocolObserver *observer,
                    NsAppLink::NsTransportManager::ITransportManager * transportManager);
        ~ProtocolHandler();

        RESULT_CODE endSession(unsigned int sessionID, unsigned int hashCode);

        RESULT_CODE sendData(const AppLinkRawMessage * message);
    
    protected:
        static void * handleMessagesFromMobileApp( void * params );
        static void * handleMessagesToMobileApp( void * params );

    private:
        struct IncomingMessage
        {
          NsAppLink::NsTransportManager::tConnectionHandle mConnectionHandle;
          unsigned int * mData;
          unsigned int mDataSize;
        };

        typedef std::multimap<unsigned char, ProtocolPacket *> PacketsMultimap;
        /**
         * @brief Processing frame received callbacks.
         *
         * @param ConnectionHandle Connection handle.
         * @param Data Received frame payload data.
         * @param DataSize Size of data in bytes.
        **/
        virtual void onFrameReceived(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);
        
        RESULT_CODE sendSingleFrameMessage(unsigned int connectionHandle,
                                      const unsigned char sessionID,
                                      const unsigned char servType,
                                      const unsigned int dataSize,
                                      const unsigned char *data,
                                      const bool compress);
        RESULT_CODE sendMultiFrameMessage(unsigned int connectionHandle,
                                         const unsigned char sessionID,
                                         const unsigned char servType,
                                         const unsigned int dataSize,
                                         const unsigned char *data,
                                         const bool compress,
                                         const unsigned int maxDataSize);
        void sendStartSessionAck( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned int protocolVersion,
              unsigned char sessionID );
        RESULT_CODE sendFrame( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              const ProtocolPacket & packet );

        RESULT_CODE handleMessage( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              ProtocolPacket& packet );
        RESULT_CODE handleMultiFrameMessage( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              ProtocolPacket & packet );
        void handleControlMessage( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              const ProtocolPacket & packet );
        

        static log4cplus::Logger mLogger;
        IProtocolObserver *mProtocolObserver;
        NsAppLink::NsTransportManager::ITransportManager * mTransportManager;
        MessageQueue<IncomingMessage> mMessagesFromMobileApp;
        pthread_t mHandleMessagesFromMobileApp;
        MessageQueue<const AppLinkRawMessage *> mMessagesToMobileApp;
        pthread_t mHandleMessagesToMobileApp;
        PacketsMultimap mIncompleteMultiFrameMessages;
        unsigned char mSessionIdCounter;
    };
}

#endif  //  PROTOCOLHANDLER_CLASS