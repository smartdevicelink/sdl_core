#ifndef PROTOCOL_HANDLER_HPP
#define PROTOCOL_HANDLER_HPP

#include <stdlib.h>
#include <map>

#include "Message.hpp"
#include "BluetoothReader.hpp"
#include "BluetoothWriter.hpp"
#include "transport/bt/IBluetoothHandler.hpp"

namespace Bluetooth
{
    class IBluetoothAPI;
}

namespace AxisCore
{

class IProtocolObserver;

/**
 * \class ProtocolHandler
 * \brief Protocol level of data exchange control
 * \author amarkosov
 */
class ProtocolHandler : public Bluetooth::IBluetoothHandler
{
public:
    /**
     * Constructor
     * @param observer Callbacks class pointer
     * @param btAdapter BT level handler class pointer
     * @param protocolVersion Protocol version
     */
    ProtocolHandler(IProtocolObserver *observer,
                    Bluetooth::IBluetoothAPI *btAdapter,
                    UInt8 protocolVersion);

    /**
      * Destructor
      */
    ~ProtocolHandler();

    /**
     * Start Session
     * @param servType Service type
     */
    ERROR_CODE startSession(UInt8 servType);

    /**
     * End Session
     * @param sessionID Id of session
     * @param hashCode Hash code received in the Start Session ACK message
     */
    ERROR_CODE endSession(UInt8 sessionID, UInt32 hashCode);

    /**
     * Send Data
     * @param sessionID Id of session
     * @param servType Service type
     * @param dataSize Data size (bytes)
     * @param data Data array
     * @param compress Compressing
     */
    ERROR_CODE sendData(UInt8 sessionID, UInt8 servType, UInt32 dataSize, UInt8 *data, bool compress);

    /**
     * Receive Data
     * @param sessionID Id of session
     * @param messageID Id of message (only protocol v.2)
     * @param servType Service type
     * @param receivedDataSize Received data size (bytes)
     * @param data Data array
     */
    ERROR_CODE receiveData(UInt8 sessionID, UInt32 messageID, UInt8 servType, UInt32 receivedDataSize, UInt8* data);

private:
    virtual void dataReceived();

    virtual void onError(BLUETOOTH_ERROR errCode);

    enum State 
    {
        HANDSHAKE_NOT_DONE,
        HANDSHAKE_DONE
    };

    ERROR_CODE sendStartAck(const UInt8 sessionID);
    ERROR_CODE handleMessage(const ProtocolPacketHeader &header, UInt8 *data);
    ERROR_CODE handleControlMessage(const ProtocolPacketHeader &header, UInt8 *data);
    ERROR_CODE handleMultiFrameMessage(const ProtocolPacketHeader &header, UInt8 *data);
    ERROR_CODE sendSingleFrameMessage(UInt8 sessionID,
                                      UInt8 servType,
                                      UInt32 dataSize,
                                      UInt8 *data,
                                      bool compress);
    ERROR_CODE sendMultiFrameMessage(UInt8 sessionID,
                                     UInt8 servType,
                                     UInt32 dataSize,
                                     UInt8 *data,
                                     bool compress,
                                     const UInt32 maxDataSize);


    IProtocolObserver *mProtocolObserver;
    std::map<UInt8, UInt8> mSessionStates;
    std::map<UInt8, UInt32> mHashCodes;
    std::map<UInt8, UInt32> mMessageCounters;
    std::map<UInt8, std::queue<Message *> > mToUpperLevelMessagesQueues;
    std::map<UInt8, Message *> mIncompleteMultiFrameMessages;
    BluetoothReader mBTReader;
    BluetoothWriter mBTWriter;
    Bluetooth::IBluetoothAPI *mBTAdapter;
    UInt8 mProtocolVersion;
};

} //namespace AxisCore

#endif //PROTOCOL_HANDLER_HPP
