/*
 * RPCBusObject.h
 *
 *  Created on: Oct 8, 2012
 *      Author: vsalo
 */

#ifndef RPCBUSOBJECT_H_
#define RPCBUSOBJECT_H_

#include <string>

namespace NsAppManager
{
	
class RPCBusObject
{
public:
	enum MessageType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };
	
	RPCBusObject( unsigned int protocolVersion, MessageType messageType, const std::string& functionName );
	RPCBusObject( const RPCBusObject& message );
	
	virtual ~RPCBusObject( );

	virtual void setParametersString( const std::string& parametersString );
	virtual std::string getParametersString() const;

	virtual unsigned int getProtocolVersion() const;
	virtual MessageType getMessageType() const;
	virtual unsigned int getCorrelationID() const;
	virtual const std::string & getFunctionName() const;
	virtual unsigned int getFunctionID() const;
	
	virtual void setCorrelationID( unsigned int correlationID );
	virtual void setFunctionName( const std::string & functionName );
	virtual void setFunctionID( unsigned int functionID );

private:
	unsigned int mProtocolVersion;
	MessageType mMessageType;
	unsigned int mCorrelationID;
	std::string mFunctionName;
	unsigned int mFunctionID;
	std::string mParametersString;
};

}; // namespace NsAppManager

#endif /* RPCBUSOBJECT_H_ */
