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

}

ERROR_CODE ProtocolHandler::startSession(SERVICE_TYPE servType)
{

}

ERROR_CODE ProtocolHandler::endSession(UInt8 sessionID)
{

}

ERROR_CODE ProtocolHandler::sendData(UInt8 sessionID
                                   , SERVICE_TYPE servType
                                   , UInt32 dataSize
                                   , UInt8 *data
                                   , bool compress)
{

}

ERROR_CODE ProtocolHandler::receiveData(UInt8 sessionID
                                      , UInt32 messageID
                                      , SERVICE_TYPE servType
                                      , UInt32 *receivedDataSize
                                      , UInt8 *data)
{

}

ERROR_CODE ProtocolHandler::sendData()
{

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
                // send ACK

                mSessionID = header.sessionID;
                mState == HANDSHAKE_DONE;
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
                //
                mState == HANDSHAKE_DONE;
            }
            break;
        }
        default:
        {}
    }
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
        /*if (header.frameData == FRAME_DATA_LAST_FRAME)
        {

        }*/
        break;
    }
    default:
    { }
    }
}

ERROR_CODE ProtocolHandler::handleMultiFrameMessage(const ProtocolPacketHeader &header
                                                  , UInt8 *data)
{
    if (header.frameType == FRAME_TYPE_FIRST)
    {
        Message multiFrameMessage(header, data, true);
        mIncompleteMultiFrameMessages.insert(std::pair<UInt32, Message>(header.messageID, multiFrameMessage) );
    }
    else
    {
        if (mIncompleteMultiFrameMessages.count(header.messageID) )
        {
            if (mIncompleteMultiFrameMessages[header.messageID].addConsecutiveMessage(header, data) == ERR_OK)
            {
                if (header.frameData == FRAME_DATA_LAST_FRAME)
                {
                    if (mProtocolObserver)
                        mProtocolObserver->dataReceivedCallback(mSessionID
                                              , header.messageID
                                              , mIncompleteMultiFrameMessages[header.messageID].getTotalDataBytes() );
                }                
            }
            else
                return ERR_FAIL;
        }   
    }
}