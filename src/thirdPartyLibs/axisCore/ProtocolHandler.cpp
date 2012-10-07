#include "ProtocolHandler.hpp"

ProtocolHandler::ProtocolHandler(IProtocolObserver *observer) : 
                mProtocolObserver(observer)
{

}

ProtocolHandler::~ProtocolHandler()
{

}

ERROR_CODE ProtocolHandler::startSession(SERVICE_TYPE servType, UInt8 &sessionID, UInt32 &hashCode)
{

}

ERROR_CODE ProtocolHandler::endSession(UInt8 sessionID, UInt32 hashCode)
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
                                      , UInt8* data)
{

}

ERROR_CODE ProtocolHandler::sendData()
{

}

ERROR_CODE ProtocolHandler::receiveData()
{
    
}
