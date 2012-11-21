#ifndef PROTOCOLHANDLER_CLASS
#define PROTOCOLHANDLER_CLASS

#include "Utils/MessageQueue.h"

namespace NsProtocolHandler
{
    class IProtocolObserver;

    enum RESULT_CODE
    {
       RESULT_OK                     = 1,
       RESULT_FAIL                   = 2,
       RESULT_NUMBER_BUSY            = 3,
       RESULT_CONNECTION_LOST        = 4,
       RESULT_DISCONNECTED           = 5,
       RESULT_NOTFOUND               = 6,
       RESULT_TIMEOUT                = 7,
       RESULT_WRONG_SEQUENCE         = 8,
       RESULT_AGAIN                  = 9,
       RESULT_WRONG_FRAME_SEQUENCE   = 10,
       RESULT_IN_PROGRESS            = 11,
       RESULT_REASSIGN               = 12,
       RESULT_XML_PARSING            = 13,
       RESULT_RESEND_ACK             = 14,
       RESULT_DEFRERRED               = 15,
       RESULT_UNKNOWN                = 255
    };

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
          unsigned int * mData;
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
      
        /**
         * Write data to device
         * @param header Message header
         * @param data Data array
         */
        RESULT_CODE sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const ProtocolPacketHeader &header, const UInt8 *data);

        static log4cplus::Logger mLogger;
        IProtocolObserver *mProtocolObserver;
        NsAppLink::NsTransportManager::ITransportManager * mTransportManager;
        MessageQueue<IncomingMessage> mMessagesFromMobileApp;
        pthread_t mHandleMessagesFromMobileApp;
        MessageQueue<const AppLinkRawMessage *> mMessagesToMobileApp;
        pthread_t mHandleMessagesToMobileApp;
    };
}

#endif  //  PROTOCOLHANDLER_CLASS