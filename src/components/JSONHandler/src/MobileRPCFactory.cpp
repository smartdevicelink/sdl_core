#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCFactory.h"
#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/OnHMIStatus.h"

MobileRPCFactory::MobileRPCFactory() {}
MobileRPCFactory::~MobileRPCFactory() {}

RegisterAppInterface MobileRPCFactory::createRegisterAppInterface ( const MobileRPCMessage & message ) const // message with params string
{
    RegisterAppInterface object( message.getProtocolVersion() );
    
    Json::Value params = JSONHandler::getParametersFromJSON( message.getParametersString() );
    object.setAppName( params["appName"].asString() );
    SyncMsgVersion syncMsgVersion;
    syncMsgVersion.majorVersion = params["syncMsgVersion"]["majorVersion"].asInt();
    syncMsgVersion.minorVersion = params["syncMsgVersion"]["minorVersion"].asInt();
    object.setSyncMsgVersion( syncMsgVersion );

    std::vector<std::string> vrSynonyms;
    for ( int i = 0; i < params["vrSynonyms"].size(); ++i )
    {
        vrSynonyms.push_back( params["vrSynonyms"][i].asString() );
    }
    object.setVrSynonyms( vrSynonyms );

    return object;
}

Json::Value MobileRPCFactory::serializeRegisterAppInterface( const RegisterAppInterface & request ) const
{
    Json::Value result;
    Json::Value value;
    const std::vector<std::string> & vrSynonyms = request.getVrSynonyms();
    if ( !vrSynonyms.empty() )
    {
        Json::Value vrSynonymsArray;
        for ( int i = 0; i < vrSynonyms.size(); ++i )
        {
            vrSynonymsArray.append( vrSynonyms[i] );
        }
        value["vrSynonyms"] = vrSynonymsArray;
    }    

    value["appName"] = request.getAppName();

    Json::Value syncMsgVersion;
    syncMsgVersion["majorVersion"] = request.getSyncMsgVersion().majorVersion;
    syncMsgVersion["minorVersion"] = request.getSyncMsgVersion().minorVersion;
    value["syncMsgVersion"] = syncMsgVersion;

    result["parameters"] = value;
    return result;
}

RegisterAppInterfaceResponse* MobileRPCFactory::createRegisterAppInterfaceResponse ( const MobileRPCMessage & message ) const 
{
    RegisterAppInterfaceResponse* object = new RegisterAppInterfaceResponse( message.getProtocolVersion() );
    object->setCorrelationID( message.getCorrelationID() );
    
    object->setSuccess( true );
    object->setResultCode( "SUCCESS" );
    
    return object;
}

Json::Value MobileRPCFactory::serializeRegisterAppInterfaceResponse( const RegisterAppInterfaceResponse & response ) const
{
    Json::Value value;
    
    value["success"] = response.getSuccess();
    value["resultCode"] = response.getResultString();

    return value;
}

SubscribeButton MobileRPCFactory::createSubscribeButton(const MobileRPCMessage &message) const
{
    SubscribeButton object( message.getProtocolVersion() );

    Json::Value params = JSONHandler::getParametersFromJSON( message.getParametersString() );
    object.setButtonName( params["buttonName"].asString() );

    return object;
}

Json::Value MobileRPCFactory::serializeSubscribeButton(const SubscribeButton &request) const
{
    Json::Value result;
    Json::Value value;

    value["buttonName"] = request.getButtonName();

    result["parameters"] = value;
    return result;
}

SubscribeButtonResponse *MobileRPCFactory::createSubscribeButtonResponse(const MobileRPCMessage &message) const
{
    SubscribeButtonResponse* object = new SubscribeButtonResponse( message.getProtocolVersion() );
    object->setCorrelationID( message.getCorrelationID() );

    object->setSuccess( true );
    object->setResultCode( "SUCCESS" );

    return object;
}

Json::Value MobileRPCFactory::serializeSubscribeButtonResponse(const SubscribeButtonResponse &response) const
{
    Json::Value value;

    value["success"] = response.getSuccess();
    value["resultCode"] = response.getResultString();

    return value;
}

UnsubscribeButton MobileRPCFactory::createUnsubscribeButton(const MobileRPCMessage &message) const
{
    UnsubscribeButton object( message.getProtocolVersion() );

    Json::Value params = JSONHandler::getParametersFromJSON( message.getParametersString() );
    object.setButtonName( params["buttonName"].asString() );

    return object;
}

Json::Value MobileRPCFactory::serializeUnsubscribeButton(const UnsubscribeButton &request) const
{
    Json::Value result;
    Json::Value value;

    value["buttonName"] = request.getButtonName();

    result["parameters"] = value;
    return result;
}

UnsubscribeButtonResponse *MobileRPCFactory::createUnsubscribeButtonResponse(const MobileRPCMessage &message) const
{
    UnsubscribeButtonResponse* object = new UnsubscribeButtonResponse( message.getProtocolVersion() );
    object->setCorrelationID( message.getCorrelationID() );

    object->setSuccess( true );
    object->setResultCode( "SUCCESS" );

    return object;
}

Json::Value MobileRPCFactory::serializeUnsubscribeButtonResponse(const UnsubscribeButtonResponse &response) const
{
    Json::Value value;

    value["success"] = response.getSuccess();
    value["resultCode"] = response.getResultString();

    return value;
}

OnHMIStatus * MobileRPCFactory::createOnHMIStatus() const
{
    OnHMIStatus * object = new OnHMIStatus(1);
    HMILevel hmiLevel = HMILevel();
    std::string str = std::string("NONE");
    hmiLevel.setHMILevel(str);
    object->setHMILevel(hmiLevel);

    AudioStreamingState audioStreamingState = AudioStreamingState();
    str = std::string("NOT_AUDIBLE");
    audioStreamingState.setAudioStreamingState(str);
    object->setAudioStreamingState(audioStreamingState);

    SystemContext systemContext = SystemContext();
    str = std::string("MAIN");
    systemContext.setSystemContext(str);
    object->setSystemContext(systemContext);
    
    return object;
}

Json::Value MobileRPCFactory::serializeOnHMIStatus( const OnHMIStatus & notification ) const
{

    Json::Value value;
    
    value["hmiLevel"] 			= notification.getHMILevel().toString();
    value["audioStreamingState"] 	= notification.getAudioStreamingState().toString();
    value["SystemContext"] 		= notification.getSystemContext().toString();

    return value;
}
