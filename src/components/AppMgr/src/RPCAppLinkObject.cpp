/*
 * RPCAppLinkObject.cpp
 *
 *  Created on: Oct 8, 2012
 *      Author: vsalo
 */

#include "AppMgr/RPCAppLinkObject.h"

namespace NsAppManager
{

RPCAppLinkObject::RPCAppLinkObject( unsigned int protocolVersion, MessageType messageType )
	:MobileRPCMessage(protocolVersion, messageType)
{
}

RPCAppLinkObject::RPCAppLinkObject( unsigned int protocolVersion, MessageType messageType,
									std::string functionName )
	:MobileRPCMessage(protocolVersion, messageType, functionName)
{
}
									
RPCAppLinkObject::RPCAppLinkObject( unsigned int protocolVersion, MessageType messageType,
									unsigned int functionID )
	:MobileRPCMessage(protocolVersion, messageType, functionID)
{
}
									
RPCAppLinkObject::RPCAppLinkObject( unsigned int protocolVersion, MessageType messageType,
				  unsigned int correlationID, std::string functionName,
				  unsigned int functionID, unsigned int messageSize )
	:MobileRPCMessage(protocolVersion, messageType, correlationID, functionName, functionID, messageSize)
{
}
				  
RPCAppLinkObject::~RPCAppLinkObject( )
{
}

};
