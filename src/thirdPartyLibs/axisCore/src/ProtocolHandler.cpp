#include <memory.h>

#include "ProtocolHandler.hpp"
#include "ProtocolPacketHeader.hpp"
#include "IProtocolObserver.hpp"

ProtocolHandler::ProtocolHandler(IProtocolObserver *observer) : 
                mProtocolObserver(observer),
                mMessageID(0),
                mSessionID(0),
                mState(BEFORE_HANDSHAKE)
{

}

ProtocolHandler::~ProtocolHandler()
{
    std::map<UInt32, Message *>::iterator it;
    for (it = mIncompleteMultiFrameMessages.begin() ; it != mIncompleteMultiFrameMessages.end() ; it++)
        delete (*it).second;

    mIncompleteMultiFrameMessages.clear();

    for (it = mOutMessagesMap.begin() ; it != mOutMessagesMap.end() ; it++)
        delete (*it).second;

    mOutMessagesMap.clear();
}

ERROR_CODE ProtocolHandler::startSession(UInt8 servType)
{
    ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                servType,
                                FRAME_DATA_START_SESSION,
                                0,
                                0,
                                0);

    mBTWriter.write(header, 0);

    return ERR_OK;
}

ERROR_CODE ProtocolHandler::endSession(UInt8 sessionID)
{
    ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                SERVICE_TYPE_RPC,
                                FRAME_DATA_END_SESSION,
                                sessionID,
                                0,
                                0);

    mBTWriter.write(header, 0);

    return ERR_OK;
}

ERROR_CODE ProtocolHandler::sendData(UInt8 sessionID
                                   , UInt8 servType
                                   , UInt32 dataSize
                                   , UInt8 *data
                                   , bool compress)
{




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

        mBTWriter.write(header, data);
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

        mBTWriter.write(firstHeader, 0);

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

                mBTWriter.write(header, outDataFrame);
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

                mBTWriter.write(header, outDataFrame);
            }
        }

        delete [] outDataFrame;
    }

    mMessageID++;

    return ERR_OK;
}

ERROR_CODE ProtocolHandler::receiveData(UInt8 sessionID
                                      , UInt32 messageID
                                      , UInt8 servType
                                      , UInt32 receivedDataSize
                                      , UInt8 *data)
{
    if (mOutMessagesMap.count(messageID) )
    {
        Message *currentMessage = mOutMessagesMap[messageID];
        if (receivedDataSize == currentMessage->getTotalDataBytes() )
        {
            memcpy(data, currentMessage->getMessageData(), receivedDataSize);

            delete currentMessage;
            mOutMessagesMap.erase(messageID);
        }
        else
            return ERR_FAIL;
    }
    else
        return ERR_FAIL;

    return ERR_OK;
}

ERROR_CODE ProtocolHandler::receiveData(const ProtocolPacketHeader &header, UInt8 *data)
{
    switch (mState)
    {
        case HANDSHAKE_DONE:
        {
            handleMessage(header, data);
            break;
        }
        case BEFORE_HANDSHAKE:
        {
            if ( (header.frameType == FRAME_TYPE_CONTROL) 
                && (header.frameData == FRAME_DATA_START_SESSION) )
            {
                sendStartAck(header.sessionID);
                mSessionID = header.sessionID;
                mState = HANDSHAKE_DONE;
                if (mProtocolObserver)
                    mProtocolObserver->sessionStartedCallback(mSessionID);
            }
            break;
        }
        case HANDSHAKE_IN_PROGRESS:
        {
            if ( (header.frameType == FRAME_TYPE_CONTROL) 
                && (header.frameData == FRAME_DATA_START_SESSION_ACK) )
            {
                mSessionID = header.sessionID;
                if (mProtocolObserver)
                    mProtocolObserver->sessionStartedCallback(mSessionID);

                mState = HANDSHAKE_DONE;
            }
            break;
        }
        default:
        {}
    }



    return ERR_OK;
}

ERROR_CODE ProtocolHandler::sendStartAck(const UInt8 sessionID)
{
    ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                SERVICE_TYPE_RPC,
                                FRAME_DATA_START_SESSION_ACK,
                                sessionID,
                                0,
                                0);

    mBTWriter.write(header, 0);

    return ERR_OK;
}

ERROR_CODE ProtocolHandler::handleMessage(const ProtocolPacketHeader &header, UInt8 *data)
{
    switch (header.frameType)
    {
    case FRAME_TYPE_CONTROL:
    {        
        if (header.frameData == FRAME_DATA_END_SESSION)
        {
            // end session

            mState = BEFORE_HANDSHAKE;
        }
        else
        {
            // incorrect message
        }
        break;
    }
    case FRAME_TYPE_SINGLE:
    {
        if (mProtocolObserver)
            mProtocolObserver->dataReceivedCallback(mSessionID, header.messageID, header.dataSize);
        break;
    }
    case FRAME_TYPE_FIRST:
    {
        handleMultiFrameMessage(header, data);
        break;
    }
    case FRAME_TYPE_CONSECUTIVE:
    {
        handleMultiFrameMessage(header, data);
        break;
    }
    default:
    { }
    }



    return ERR_OK;
}

ERROR_CODE ProtocolHandler::handleMultiFrameMessage(const ProtocolPacketHeader &header
                                                  , UInt8 *data)
{
    if (header.frameType == FRAME_TYPE_FIRST)
    {
        Message *multiFrameMessage = new Message(header, data, true);
        mIncompleteMultiFrameMessages.insert(std::pair<UInt32, Message*>(header.messageID, multiFrameMessage) );
    }
    else
    {
        if (mIncompleteMultiFrameMessages.count(header.messageID) )
        {
            if (mIncompleteMultiFrameMessages[header.messageID]->addConsecutiveMessage(header, data) == ERR_OK)
            {
                if (header.frameData == FRAME_DATA_LAST_FRAME)
                {
                    mOutMessagesMap.insert(std::pair<UInt32, Message*>(header.messageID
                                                                       , mIncompleteMultiFrameMessages[header.messageID] ) );
                    mIncompleteMultiFrameMessages.erase(header.messageID);

                    if (mProtocolObserver)
                        mProtocolObserver->dataReceivedCallback(mSessionID
                                              , header.messageID
                                              , mOutMessagesMap[header.messageID]->getTotalDataBytes() );
                }                
            }
            else
                return ERR_FAIL;
        }   
    }

    return ERR_OK;
}
