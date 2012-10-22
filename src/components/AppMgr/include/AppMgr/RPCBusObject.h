/*
 * RPCBusObject.h
 *
 *  Created on: Oct 8, 2012
 *      Author: vsalo
 */

#ifndef RPCBUSOBJECT_H_
#define RPCBUSOBJECT_H_

#include <string>
#include <map>

namespace NsAppManager
{
	
class RPCBusObject
{
public:
	typedef std::map<std::string, std::string> Parameters;
	typedef std::pair<std::string, std::string> Parameter;
	
	enum MessageType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };
	
	RPCBusObject( unsigned int protocolVersion, MessageType messageType, const std::string& method );
	
	virtual ~RPCBusObject( );

	virtual unsigned int getProtocolVersion() const;
	virtual MessageType getMessageType() const;
	virtual unsigned int getCorrelationID() const;
	virtual const std::string & getMethodName() const;
	virtual unsigned int getFunctionID() const;
	
	virtual void setCorrelationID( unsigned int correlationID );
	virtual void setFunctionName( const std::string & functionName );
	virtual void setFunctionID( unsigned int functionID );
	virtual void setParameter( const std::string& param, const std::string& value );
	virtual std::string getParameter( const std::string& param );
	virtual const Parameters& getParameters() const;

private:
	RPCBusObject( const RPCBusObject& message );
	
	unsigned int mProtocolVersion;
	MessageType mMessageType;
	unsigned int mCorrelationID;
	std::string mFunctionName;
	unsigned int mFunctionID;
	Parameters mParameters;
};

}; // namespace NsAppManager

#endif /* RPCBUSOBJECT_H_ */
