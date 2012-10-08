#ifndef PROTOCOL_HANDLER_HPP
#define PROTOCOL_HANDLER_HPP

#include <stdlib.h>
#include <map>

#include "Message.hpp"

class IProtocolObserver;

class ProtocolHandler
{
public:

    ProtocolHandler(IProtocolObserver *observer);

    ~ProtocolHandler();

    /**
     * Start Session
     * @param servType Service type, always RPC (0x07)
     */
    ERROR_CODE startSession(UInt8 servType);

    /**
     * End Session
     * @param sessionID Id of session
     */
    ERROR_CODE endSession(UInt8 sessionID);

    /**
     * Send Data
     * @param sessionID Id of session
     * @param servType Service type (always RPC (0x07) )
     * @param dataSize Data size (bytes)
     * @param data Data array
     * @param compress Compressing (always 0)
     */
    ERROR_CODE sendData(UInt8 sessionID, UInt8 servType, UInt32 dataSize, UInt8 *data, bool compress);

    /**
     * Receive Data
     * @param sessionID Id of session
     * @param messageID Id of message
     * @param servType Service type (always RPC (0x07) )
     * @param receivedDataSize Received data size (bytes)
     * @param data Data array
     */
    ERROR_CODE receiveData(UInt8 sessionID, UInt32 messageID, UInt8 servType, UInt32 receivedDataSize, UInt8* data);

private:
    enum State 
    {
        BEFORE_HANDSHAKE,
        HANDSHAKE_IN_PROGRESS,
        HANDSHAKE_DONE
    };

    ERROR_CODE sendData();
    ERROR_CODE receiveData(const ProtocolPacketHeader &header, UInt8 *data);
    ERROR_CODE sendStartAck(const UInt8 sessionID);
    ERROR_CODE handleMessage(const ProtocolPacketHeader &header, UInt8 *data);
    ERROR_CODE handleMultiFrameMessage(const ProtocolPacketHeader &header, UInt8 *data);

    IProtocolObserver *mProtocolObserver;
    UInt8 mState;
    UInt8 mSessionID;
    UInt32 mMessageID;
    std::map<UInt32, Message *> mOutMessagesMap;
    //std::map<UInt32, Message *> mInMessagesMap;
    std::map<UInt32, Message *> mIncompleteMultiFrameMessages;
};

#endif //PROTOCOL_HANDLER_HPP
