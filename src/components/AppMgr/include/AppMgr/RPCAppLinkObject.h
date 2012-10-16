/*
 * RPCAppLinkObject.h
 *
 *  Created on: Oct 8, 2012
 *      Author: vsalo
 */

#ifndef RPCAPPLINKOBJECT_H_
#define RPCAPPLINKOBJECT_H_

#include "JSONHandler/MobileRPCMessage.h"

namespace NsAppManager
{
	
class RPCAppLinkObject: public MobileRPCMessage
{
public:
	RPCAppLinkObject( unsigned int protocolVersion, MessageType messageType );
	RPCAppLinkObject( unsigned int protocolVersion, MessageType messageType,
					  std::string functionName );
	RPCAppLinkObject( unsigned int protocolVersion, MessageType messageType,
					  unsigned int functionID );
	RPCAppLinkObject( unsigned int protocolVersion, MessageType messageType,
					  unsigned int correlationID, std::string functionName,
				   unsigned int functionID, unsigned int messageSize );
	virtual ~RPCAppLinkObject( );
};

}; // namespace NsAppManager

#endif /* RPCAPPLINKOBJECT_H_ */
