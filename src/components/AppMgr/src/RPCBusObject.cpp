/*
 * RPCBusObject.cpp
 *
 *  Created on: Oct 8, 2012
 *      Author: vsalo
 */

#include "AppMgr/RPCBusObject.h"

namespace NsAppManager
{
	
RPCBusObject::RPCBusObject( unsigned int protocolVersion, MessageType messageType, const std::string& functionName )
	:mProtocolVersion( protocolVersion )
	,mMessageType( messageType )
	,mCorrelationID( 0 )
	,mFunctionName( functionName )
	,mFunctionID( 0 )
{
	// TODO Auto-generated constructor stub

}

RPCBusObject::RPCBusObject( const RPCBusObject& message )
	:mProtocolVersion( message.getProtocolVersion() )
	,mMessageType( message.getMessageType() )
	,mCorrelationID( message.getCorrelationID() )
	,mFunctionName( message.getFunctionName() )
	,mFunctionID( message.getFunctionID() )
{}

RPCBusObject::~RPCBusObject( )
{
	// TODO Auto-generated destructor stub
}

unsigned int RPCBusObject::getProtocolVersion() const
{
	return mProtocolVersion;
}

RPCBusObject::MessageType RPCBusObject::getMessageType() const
{
	return mMessageType;
}

unsigned int RPCBusObject::getCorrelationID() const
{
	return mCorrelationID;
}

const std::string & RPCBusObject::getFunctionName() const
{
	return mFunctionName;
}

unsigned int RPCBusObject::getFunctionID() const
{
	return mFunctionID;
}

void RPCBusObject::setFunctionName( const std::string & functionName )
{
	mFunctionName = functionName;
}

void RPCBusObject::setFunctionID( unsigned int functionID )
{
	mFunctionID = functionID;
}

void RPCBusObject::setCorrelationID( unsigned int correlationID )
{
	mCorrelationID = correlationID;
}


void RPCBusObject::setParametersString( const std::string& parametersString )
{
	mParametersString = parametersString;
}

std::string RPCBusObject::getParametersString() const
{
	return mParametersString;
}

};
