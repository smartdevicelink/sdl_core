

MobileRPCMessage( uint protocolVersion, MessageType messageType, 
        uint correlationID = 0 )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( correlationID )
,mFunctionName( "" )
,mFunctionID( 0 )
,mJSONMessageSize( 0 )
{}

MobileRPCMessage::MobileRPCMessage( uint protocolVersion, MessageType messageType, 
        uint correlationID, Parameters params, std::string functionName )
{
    MobileRPCMessage(protocolVersion, messageType, 
        correlationID, params, functionName, 0, 0);
}

MobileRPCMessage::MobileRPCMessage( uint protocolVersion, MessageType messageType, 
        uint correlationID, Parameters params,
        uint functionID, uint messageSize )
{
    MobileRPCMessage(protocolVersion, messageType, 
        correlationID, params, "", functionID, messageSize);
}

MobileRPCMessage::MobileRPCMessage( uint protocolVersion, MessageType messageType, 
        uint correlationID, Parameters params, std::string functionName,
        uint functionID, uint messageSize )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( correlationID )
,mParameters( params )
,mFunctionName( functionName )
,mFunctionID( functionID )
,mJSONMessageSize( messageSize )
{ }

