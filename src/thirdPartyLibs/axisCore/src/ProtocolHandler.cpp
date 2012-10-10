#include <memory.h>
#include <iostream>

#include "ProtocolHandler.hpp"
#include "ProtocolPacketHeader.hpp"
#include "IProtocolObserver.hpp"
//#include "transport/bt/BluetoothAPI.hpp"
#include "MessageGenerator/CMessage.cpp"

ProtocolHandler::ProtocolHandler(IProtocolObserver *observer) : 
                mProtocolObserver(observer),
                mMessageID(0),
                mSessionID(0),
                mState(BEFORE_HANDSHAKE)
{
    std::cout << "enter ProtocolHandler::ProtocolHandler() \n";
    //

    //Bluetooth::initBluetooth(this);
}

ProtocolHandler::~ProtocolHandler()
{
    std::cout << "enter ProtocolHandler::~ProtocolHandler() \n";

    std::map<UInt32, Message *>::iterator it;
    for (it = mIncompleteMultiFrameMessages.begin() ; it != mIncompleteMultiFrameMessages.end() ; it++)
        delete (*it).second;

    mIncompleteMultiFrameMessages.clear();

    for (it = mToUpperLevelMessagesMap.begin() ; it != mToUpperLevelMessagesMap.end() ; it++)
        delete (*it).second;

    mToUpperLevelMessagesMap.clear();

    std::cout << "exit ProtocolHandler::~ProtocolHandler() \n";
}

ERROR_CODE ProtocolHandler::startSession(UInt8 servType)
{
    ERROR_CODE retVal = ERR_OK;

    ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                servType,
                                FRAME_DATA_START_SESSION,
                                0,
                                0,
                                0);

    if (mBTWriter.write(header, 0) == ERR_OK)
        std::cout << "ProtocolHandler::startSession return OK \n";
    else
    {
        std::cout << "ProtocolHandler::startSession return FAIL \n";
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::endSession(UInt8 sessionID)
{
    ERROR_CODE retVal = ERR_OK;

    ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                SERVICE_TYPE_RPC,
                                FRAME_DATA_END_SESSION,
                                sessionID,
                                0,
                                0);

    if (mBTWriter.write(header, 0) == ERR_OK)
    {
        std::cout << "ProtocolHandler::endSession return OK \n";
    }
    else
    {
        std::cout << "ProtocolHandler::endSession return FAIL \n";
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::sendData(UInt8 sessionID
                                   , UInt8 servType
                                   , UInt32 dataSize
                                   , UInt8 *data
                                   , bool compress)
{
    std::cout << "enter ProtocolHandler::sendData() \n";



    //TODO maxsize
    const unsigned int MAXIMUM_DATA_SIZE = 5000;





    if (dataSize <= MAXIMUM_DATA_SIZE)
    {
        ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                    compress,
                                    FRAME_TYPE_SINGLE,
                                    servType,
                                    0,
                                    sessionID,
                                    dataSize,
                                    mMessageID);

        if (!(mBTWriter.write(header, data) == ERR_OK) )
        {
            std::cout << "ProtocolHandler::sendData() write single frame FAIL \n";
            return ERR_FAIL;
        }
        else
            std::cout << "ProtocolHandler::sendData() write single frame OK \n";
    }
    else
    {
        int numOfFrames = 0;
        int subDataSize = dataSize / MAXIMUM_DATA_SIZE;
        int lastDataSize = subDataSize;

        if (dataSize % MAXIMUM_DATA_SIZE)
        {
            numOfFrames = (dataSize / MAXIMUM_DATA_SIZE) + 1;
            lastDataSize = dataSize % MAXIMUM_DATA_SIZE;
        }
        else
            numOfFrames = dataSize / MAXIMUM_DATA_SIZE;

        ProtocolPacketHeader firstHeader(PROTOCOL_VERSION_1,
                                    compress,
                                    FRAME_TYPE_FIRST,
                                    servType,
                                    0,
                                    sessionID,
                                    FIRST_FRAME_DATA_SIZE,
                                    mMessageID);

        UInt8 *outDataFirstFrame = new UInt8[FIRST_FRAME_DATA_SIZE];
        ( (UInt32*)outDataFirstFrame)[0] = dataSize;
        ( (UInt32*)outDataFirstFrame)[1] = numOfFrames;

        if (!(mBTWriter.write(firstHeader, 0) == ERR_OK) )
        {
            std::cout << "ProtocolHandler::sendData() write first frame FAIL \n";
            return ERR_FAIL;
        }
        else
            std::cout << "ProtocolHandler::sendData() write first frame OK \n";

        delete [] outDataFirstFrame;


        int frameDataMaxValue = 0xFF;

        UInt8 *outDataFrame = new UInt8[subDataSize];

        for (UInt8 i = 0 ; i <= numOfFrames ; i++)
        {
            if (i != numOfFrames)
            {
                ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                            compress,
                                            FRAME_TYPE_CONSECUTIVE,
                                            servType,
                                            ( (i % frameDataMaxValue) + 1),
                                            sessionID,
                                            subDataSize,
                                            mMessageID);

                memcpy(outDataFrame, data + (subDataSize * i), subDataSize);

                if ( mBTWriter.write(header, outDataFrame) == ERR_FAIL)
                {
                    std::cout << "ProtocolHandler::sendData() write consecutive frame FAIL \n";
                    return ERR_FAIL;
                }
                else
                    std::cout << "ProtocolHandler::sendData() write consecutive frame OK \n";
            }
            else
            {
                ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                            compress,
                                            FRAME_TYPE_CONSECUTIVE,
                                            servType,
                                            0x0,
                                            sessionID,
                                            lastDataSize,
                                            mMessageID);

                memcpy(outDataFrame, data + (subDataSize * i), lastDataSize);

                if (mBTWriter.write(header, outDataFrame) == ERR_FAIL)
                {
                    std::cout << "ProtocolHandler::sendData() write last frame FAIL \n";
                    return ERR_FAIL;
                }
                else
                    std::cout << "ProtocolHandler::sendData() write last frame OK \n";
            }
        }

        delete [] outDataFrame;
    }

    mMessageID++;

    return ERR_OK;
}

ERROR_CODE ProtocolHandler::ReceiveData(UInt8 sessionID
                                      , UInt32 messageID
                                      , UInt8 servType
                                      , UInt32 receivedDataSize
                                      , UInt8 *data)
{
    std::cout << "enter ProtocolHandler::receiveData() \n";
    ERROR_CODE retVal = ERR_OK;

    if (mToUpperLevelMessagesMap.count(messageID) )
    {
        Message *currentMessage = mToUpperLevelMessagesMap[messageID];
        if (receivedDataSize == currentMessage->getTotalDataBytes() )
        {
            memcpy(data, currentMessage->getMessageData(), receivedDataSize);

            delete currentMessage;
            mToUpperLevelMessagesMap.erase(messageID);
        }
        else
            retVal = ERR_FAIL;
    }
    else
        retVal = ERR_FAIL;

    return retVal;
}

ERROR_CODE ProtocolHandler::handleReceivedData(const ProtocolPacketHeader &header, UInt8 *data)
{
    std::cout << "enter ProtocolHandler::handleReceivedData() \n";
    ERROR_CODE retVal = ERR_OK;

    switch (mState)
    {
        case HANDSHAKE_DONE:
        {
            std::cout << "ProtocolHandler::handleReceivedData() case HANDSHAKE_DONE \n";
            if (!(handleMessage(header, data) == ERR_OK) )
            {
                std::cout << "ProtocolHandler::handleReceivedData() case HANDSHAKE_DONE handleMessage FAIL\n";
                retVal = ERR_FAIL;
            }
            break;
        }
        case BEFORE_HANDSHAKE:
        {
            std::cout << "ProtocolHandler::handleReceivedData() case BEFORE_HANDSHAKE \n";

            if ( (header.frameType == FRAME_TYPE_CONTROL) 
                && (header.frameData == FRAME_DATA_START_SESSION) )
            {
                sendStartAck(header.sessionID);
                mSessionID = header.sessionID;
                mState = HANDSHAKE_DONE;
                if (mProtocolObserver)
                    mProtocolObserver->sessionStartedCallback(mSessionID);
                else
                {
                    std::cout << "ProtocolHandler::handleReceivedData() invalid mProtocolObserver ptr\n";
                    retVal = ERR_FAIL;
                }
            }
            else
                std::cout << "ProtocolHandler::handleReceivedData() case BEFORE_HANDSHAKE. \
                             Incorrect message for this case\n";

            break;
        }
        case HANDSHAKE_IN_PROGRESS:
        {
            std::cout << "ProtocolHandler::handleReceivedData() case HANDSHAKE_IN_PROGRESS \n";

            if ( (header.frameType == FRAME_TYPE_CONTROL) 
                && (header.frameData == FRAME_DATA_START_SESSION_ACK) )
            {
                mSessionID = header.sessionID;
                if (mProtocolObserver)
                    mProtocolObserver->sessionStartedCallback(mSessionID);
                else
                {
                    std::cout << "ProtocolHandler::handleReceivedData() invalid mProtocolObserver ptr\n";
                    retVal = ERR_FAIL;
                }

                mState = HANDSHAKE_DONE;
            }
            break;
        }
        default:
        {}
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::sendStartAck(const UInt8 sessionID)
{
    std::cout << "enter ProtocolHandler::sendStartAck \n";
    ERROR_CODE retVal = ERR_OK;

    ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                SERVICE_TYPE_RPC,
                                FRAME_DATA_START_SESSION_ACK,
                                sessionID,
                                0,
                                0);

    if (mBTWriter.write(header, 0) == ERR_OK)
        std::cout << "ProtocolHandler::sendStartAck() write OK \n";
    else
    {
        std::cout << "enter ProtocolHandler::sendStartAck() write FAIL \n";
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::handleMessage(const ProtocolPacketHeader &header, UInt8 *data)
{
    std::cout << "enter ProtocolHandler::handleMessage() \n";
    ERROR_CODE retVal = ERR_OK;

    switch (header.frameType)
    {
    case FRAME_TYPE_CONTROL:
    {        
        std::cout << "ProtocolHandler::handleMessage() case FRAME_TYPE_CONTROL \n";
        if (header.frameData == FRAME_DATA_END_SESSION)
        {
            std::cout << "ProtocolHandler::handleMessage() end session message \n";
            if (mProtocolObserver)
                mProtocolObserver->sessionEndedCallback(header.sessionID);
            mState = BEFORE_HANDSHAKE;
        }
        else
        {
            std::cout << "ProtocolHandler::handleMessage() incorrect message (FRAME_TYPE_CONTROL, but not \"end session\")\n";
            retVal = ERR_FAIL;
        }
        break;
    }
    case FRAME_TYPE_SINGLE:
    {
        std::cout << "ProtocolHandler::handleMessage() case FRAME_TYPE_SINGLE \n";
        Message *message = new Message(header, data, false);
        delete [] data;
        mToUpperLevelMessagesMap.insert(std::pair<UInt32, Message*>(header.messageID, message) );

        if (mProtocolObserver)
            mProtocolObserver->dataReceivedCallback(mSessionID, header.messageID, header.dataSize);
        else
        {
            std::cout << "ProtocolHandler::handleMessage() invalid mProtocolObserver pointer \n";
            retVal = ERR_FAIL;
        }
        break;
    }
    case FRAME_TYPE_FIRST:
    {
        std::cout << "ProtocolHandler::handleMessage() case FRAME_TYPE_FIRST \n";
        handleMultiFrameMessage(header, data);
        delete [] data;
        break;
    }
    case FRAME_TYPE_CONSECUTIVE:
    {
        std::cout << "ProtocolHandler::handleMessage() case FRAME_TYPE_CONSECUTIVE \n";
        handleMultiFrameMessage(header, data);
        delete [] data;
        break;
    }
    default:
    {
        std::cout << "ProtocolHandler::handleMessage() case !!!default!!! \n";
    }
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::handleMultiFrameMessage(const ProtocolPacketHeader &header
                                                  , UInt8 *data)
{
    ERROR_CODE retVal = ERR_OK;
    std::cout << "enter ProtocolHandler::handleMultiFrameMessage(); \n";

    if (header.frameType == FRAME_TYPE_FIRST)
    {
        std::cout << "ProtocolHandler::handleMultiFrameMessage() : FRAME_TYPE_FIRST\n";
        Message *multiFrameMessage = new Message(header, data, true);
        mIncompleteMultiFrameMessages.insert(std::pair<UInt32, Message*>(header.messageID, multiFrameMessage) );
    }
    else
    {
        std::cout << "ProtocolHandler::handleMultiFrameMessage() : !FRAME_TYPE_FIRST\n";
        if (mIncompleteMultiFrameMessages.count(header.messageID) )
        {
            if (mIncompleteMultiFrameMessages[header.messageID]->addConsecutiveMessage(header, data) == ERR_OK)
            {
                if (header.frameData == FRAME_DATA_LAST_FRAME)
                {
                    mToUpperLevelMessagesMap.insert(std::pair<UInt32, Message*>(header.messageID
                                                         , mIncompleteMultiFrameMessages[header.messageID] ) );
                    mIncompleteMultiFrameMessages.erase(header.messageID);

                    if (mProtocolObserver)
                        mProtocolObserver->dataReceivedCallback(mSessionID
                                              , header.messageID
                                              , mToUpperLevelMessagesMap[header.messageID]->getTotalDataBytes() );
                    else
                    {
                        std::cout << "ProtocolHandler::handleMultiFrameMessage() invalid mProtocolObserver pointer \n";
                        retVal = ERR_FAIL;
                    }
                }                
            }
            else
            {
                std::cout << "ProtocolHandler::handleMultiFrameMessage() : addConsecutiveMessage FAIL \n";
                retVal = ERR_FAIL;
            }
        }   
    }

    return retVal;
}

/**
  * Bluetooth Callbacks
  */

void ProtocolHandler::onError(BLUETOOTH_ERROR errCode)
{
    std::cout << "ProtocolHandler::onError(BLUETOOTH_ERROR errCode) : " << errCode << "\n";
}

void ProtocolHandler::dataReceived()
{
    std::cout << "enter ProtocolHandler::dataReceived()\n";

    UInt32 dataSize = Bluetooth::getBuffer().size(); //CMessage::currentBlob.size();
    UInt8 *data = new UInt8[dataSize];
    ProtocolPacketHeader header;

    if (mBTReader.read(header, data, dataSize) == ERR_OK)
        handleReceivedData(header, data);
    else
        std::cout << "ProtocolHandler::dataReceived() error reading\n";
}
