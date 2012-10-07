#include "JSONHandler/MobileRPCMessage.h"


MobileRPCMessage::MobileRPCMessage( 
    unsigned int protocolVersion, MessageType messageType )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( 0 )
,mFunctionName( "" )
,mFunctionID( 0 )
,mJSONMessageSize( 0 )
{}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, 
    MessageType messageType, std::string functionName )
{
    MobileRPCMessage(protocolVersion, messageType, 
        0, Parameters(), functionName, 0, 0);
}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, 
    MessageType messageType, unsigned int functionID )
{
    MobileRPCMessage(protocolVersion, messageType, 
        0, Parameters(), "", functionID, 0);
}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, Parameters params, std::string functionName,
        unsigned int functionID, unsigned int messageSize )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( correlationID )
,mParameters( params )
,mFunctionName( functionName )
,mFunctionID( functionID )
,mJSONMessageSize( messageSize )
{ }

MobileRPCMessage::~MobileRPCMessage() {}

void MobileRPCMessage::setParameters( Parameters params )
{
    mParameters.clear();
    mParameters = params;
}
    
void MobileRPCMessage::addParameter( std::string key, std::string value )
{
    mParameters[ key ] = value;
}
    
void MobileRPCMessage::changeParameter( std::string key, std::string newValue )
{
    MobileRPCMessage::Parameters::iterator it = mParameters.find( key );
    if ( it != mParameters.end() )
    {
        it->second = newValue;
    }
}

MobileRPCMessage::Parameters MobileRPCMessage::getParameters() const
{
    return mParameters;
}
    
std::string MobileRPCMessage::getParameter( std::string key ) const
{
    MobileRPCMessage::Parameters::const_iterator it = mParameters.find( key );
    if ( it == mParameters.end() ) 
    {
        return "";
    } 
    else
    {
        return it->second;
    }
}

unsigned int MobileRPCMessage::getProtocolVersion() const
{
    return mProtocolVersion;
}
    
MobileRPCMessage::MessageType MobileRPCMessage::getMessageType() const
{
    return mMessageType;
}
    
unsigned int MobileRPCMessage::getCorrelationID() const
{
    return mCorrelationID;
}
    
const std::string & MobileRPCMessage::getFunctionName() const
{
    return mFunctionName;
}
    
unsigned int MobileRPCMessage::getFunctionID() const
{
    return mFunctionID;
}
    
unsigned int MobileRPCMessage::getJSONMessageSize() const
{
    return mJSONMessageSize;
}

void MobileRPCMessage::setFunctionName( const std::string & functionName )
{
    mFunctionName = functionName;
}
    
void MobileRPCMessage::setFunctionID( unsigned int functionID )
{
    mFunctionID = functionID;
}
    
void MobileRPCMessage::setJSONMessageSize( unsigned int messageSize )
{
    mJSONMessageSize = messageSize;
}

void MobileRPCMessage::setCorrelationID( unsigned int correlationID )
{
    mCorrelationID = correlationID;
}