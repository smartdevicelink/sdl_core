#ifndef PROTOCOL_HANDLER_HPP
#define PROTOCOL_HANDLER_HPP

class IProtocolObserver;

class ProtocolHandler
{
public:

    ProtocolHandler(IProtocolObserver *observer);

    ~ProtocolHandler();

    /**
     * Start Session
     * @param servType - always RPC (0x07)
     * @param sessionID returned in the StartSessionACK message
     * @param hashCode returned in the StartSessionACK message
     */
    ERROR_CODE startSession(SERVICE_TYPE servType, UInt8 &sessionID, UInt32 &hashCode);

    /**
     * End Session
     * @param sessionID - id of session
     * @param hashCode - code, received in startSession()
     */
    ERROR_CODE endSession(UInt8 sessionID, UInt32 hashCode);

    /**
     * Send Data
     * @param sessionID - id of session
     * @param servType - service type (always RPC (0x07) )
     * @param dataSize - data size (bytes)
     * @param data - data array
     * @param compress - compressing (always 0)
     */
    ERROR_CODE sendData(UInt8 sessionID, SERVICE_TYPE servType, UInt32 dataSize, UInt8 *data, bool compress);

    /**
     * Receive Data
     * @param sessionID - id of session
     * @param messageID - id of message
     * @param servType - service type (always RPC (0x07) )
     * @param receivedDataSize - received data size (bytes)
     * @param data - data array
     */
    ERROR_CODE receiveData(UInt8 sessionID, UInt32 messageID, SERVICE_TYPE servType, UInt32 *receivedDataSize, UInt8* data);

private:
    ERROR_CODE sendData();
    ERROR_CODE receiveData();

    IProtocolObserver *mProtocolObserver;

};

#endif //PROTOCOL_HANDLER_HPP
