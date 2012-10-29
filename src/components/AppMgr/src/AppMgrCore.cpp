/*
 * AppMgr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrCore.h"
#include "AppMgr/Application.h"
#include "AppMgr/AppMgrRegistry.h"
#include "AppMgr/AppPolicy.h"
#include "AppMgr/RegistryItem.h"
#include "JSONHandler/ALRPCMessage.h"
#include "JSONHandler/ALRPCRequest.h"
#include "JSONHandler/ALRPCResponse.h"
#include "JSONHandler/ALRPCNotification.h"
#include "JSONHandler/ALRPCObjects/Marshaller.h"
#include "JSONHandler/ALRPCObjects/RegisterAppInterface_request.h"
#include "JSONHandler/ALRPCObjects/RegisterAppInterface_response.h"
#include "JSONHandler/ALRPCObjects/SubscribeButton_request.h"
#include "JSONHandler/ALRPCObjects/SubscribeButton_response.h"
#include "JSONHandler/ALRPCObjects/UnsubscribeButton_request.h"
#include "JSONHandler/ALRPCObjects/UnsubscribeButton_response.h"
#include "JSONHandler/ALRPCObjects/ButtonCapabilities.h"
#include "JSONHandler/ALRPCObjects/AppInterfaceUnregisteredReason.h"
#include "JSONHandler/ALRPCObjects/OnButtonEvent.h"
#include "JSONHandler/ALRPCObjects/OnButtonPress.h"
#include "JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered.h"
#include "JSONHandler/ALRPCObjects/OnHMIStatus.h"
#include "JSONHandler/ALRPCObjects/HMILevel.h"
#include "JSONHandler/ALRPCObjects/SetGlobalProperties_request.h"
#include "JSONHandler/ALRPCObjects/SetGlobalProperties_response.h"
#include "JSONHandler/ALRPCObjects/ResetGlobalProperties_request.h"
#include "JSONHandler/ALRPCObjects/ResetGlobalProperties_response.h"
#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"
#include "JSONHandler/OnButtonEvent.h"
#include "JSONHandler/RPC2Marshaller.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/GetCapabilitiesResponse.h"
#include "JSONHandler/SetGlobalProperties.h"
#include "JSONHandler/ResetGlobalProperties.h"
#include "JSONHandler/OnAppRegistered.h"
#include "JSONHandler/OnAppUnregistered.h"
#include <sys/socket.h>
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger AppMgrCore::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore"));
	
AppMgrCore& AppMgrCore::getInstance( )
{
	static AppMgrCore appMgr;
	return appMgr;
}

AppMgrCore::AppMgrCore()
	:mThreadRPCAppLinkObjectsIncoming(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCAppLinkObjectsIncoming, NULL))
	,mThreadRPCAppLinkObjectsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCAppLinkObjectsOutgoing, NULL))
	,mThreadRPCBusObjectsIncoming(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCBusObjectsIncoming, NULL))
	,mThreadRPCBusObjectsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCBusObjectsOutgoing, NULL))
    ,mThreadMobileRPCNotificationsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueMobileRPCNotificationsOutgoing, NULL))
	,m_bTerminate(false)
    ,mJSONHandler(0)
    ,mJSONRPC2Handler(0)
{
    LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore constructed!");
}

AppMgrCore::AppMgrCore(const AppMgrCore &)
    :mThreadRPCAppLinkObjectsIncoming(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCAppLinkObjectsIncoming, NULL))
    ,mThreadRPCAppLinkObjectsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCAppLinkObjectsOutgoing, NULL))
    ,mThreadRPCBusObjectsIncoming(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCBusObjectsIncoming, NULL))
    ,mThreadRPCBusObjectsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCBusObjectsOutgoing, NULL))
    ,mThreadMobileRPCNotificationsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueMobileRPCNotificationsOutgoing, NULL))
    ,m_bTerminate(false)
    ,mJSONHandler(0)
    ,mJSONRPC2Handler(0)
{
}

AppMgrCore::~AppMgrCore()
{
	if(!mThreadRPCAppLinkObjectsIncoming.Join())
		mThreadRPCAppLinkObjectsIncoming.Stop();
	
	if(!mThreadRPCAppLinkObjectsOutgoing.Join())
		mThreadRPCAppLinkObjectsOutgoing.Stop();
	
	if(!mThreadRPCBusObjectsIncoming.Join())
		mThreadRPCBusObjectsIncoming.Stop();
	
	if(!mThreadRPCBusObjectsOutgoing.Join())
		mThreadRPCBusObjectsOutgoing.Stop();

	LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore destructed!");
}

void AppMgrCore::pushMobileRPCMessage( ALRPCMessage * message, unsigned char sessionID )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Pushing mobile RPC message...");
	mMtxRPCAppLinkObjectsIncoming.Lock();
	
    mQueueRPCAppLinkObjectsIncoming.push(Message(message, sessionID));
	
	mMtxRPCAppLinkObjectsIncoming.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Pushed mobile RPC message");
}

void AppMgrCore::pushRPC2CommunicationMessage( RPC2Communication::RPC2Command * message )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Returning a message from HMI...");
	mMtxRPCBusObjectsIncoming.Lock();
	
	mQueueRPCBusObjectsIncoming.push(message);
	
	mMtxRPCBusObjectsIncoming.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Returned a message from HMI");
}

void AppMgrCore::executeThreads()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");
	mThreadRPCAppLinkObjectsIncoming.Start(false);
	mThreadRPCAppLinkObjectsOutgoing.Start(false);
	mThreadRPCBusObjectsIncoming.Start(false);
	mThreadRPCBusObjectsOutgoing.Start(false);
    mThreadMobileRPCNotificationsOutgoing.Start(false);

	LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
}

void AppMgrCore::terminateThreads()
{
	m_bTerminate = true;
}

void AppMgrCore::handleMobileRPCMessage( const Message& message )
{
    unsigned char sessionID = message.second;
    LOG4CPLUS_INFO_EXT(mLogger, " A mobile RPC message "<< message.first->getMethodId() <<" has been received!");
    switch(message.first->getMethodId())
	{
		case Marshaller::METHOD_REGISTERAPPINTERFACE_REQUEST:
		{
			LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface request has been invoked");
            RegisterAppInterface_request * object = (RegisterAppInterface_request*)message.first;
            const RegistryItem* registeredApp =  registerApplication( message );
            const ALRPCMessage* info = queryInfoForRegistration( registeredApp );
            RegisterAppInterface_response* response = new RegisterAppInterface_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(ALRPCMessage::RESPONSE);
            if(object->get_autoActivateID())
            {
                response->set_autoActivateID(*object->get_autoActivateID());
            }
            response->set_buttonCapabilities(getButtonCapabilities());
            if(registeredApp)
            {
                response->set_success(true);
                response->set_resultCode(Result::SUCCESS);
            }
            else
            {
                response->set_success(false);
                response->set_resultCode(Result::APPLICATION_NOT_REGISTERED);
            }
            Message responseMessage = Message(response, sessionID);
            sendMobileRPCResponse( responseMessage );
            if(registeredApp)
            {
                const Application* app = registeredApp->getApplication();
                OnHMIStatus* status = new OnHMIStatus();
                status->set_hmiLevel(app->getApplicationHMIStatusLevel());
                sendMobileRPCResponse(Message(status, sessionID));
                RPC2Communication::OnAppRegistered* appRegistered = new RPC2Communication::OnAppRegistered();
                appRegistered->setAppName(app->getName());
                appRegistered->setIsMediaApplication(app->getIsMediaApplication());
                appRegistered->setLanguageDesired(app->getLanguageDesired());
                appRegistered->setVrSynonyms(app->getVrSynonyms());
                sendHMIRPC2Response(appRegistered);
            }

            break;
		}
        case Marshaller::METHOD_UNREGISTERAPPINTERFACE_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An UnregisterAppInterface request has been invoked");

            UnregisterAppInterface_request * object = (UnregisterAppInterface_request*)message.first;
            std::string appName = AppMgrRegistry::getInstance().getItem(message.second)->getApplication()->getName();
            unregisterApplication( message );
            UnregisterAppInterface_response* response = new UnregisterAppInterface_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(Result::SUCCESS);
            Message responseMessage = Message(response, sessionID);
            sendMobileRPCResponse( responseMessage );
            OnAppInterfaceUnregistered* msgUnregistered = new OnAppInterfaceUnregistered();
            msgUnregistered->set_reason(AppInterfaceUnregisteredReason(AppInterfaceUnregisteredReason::USER_EXIT));
            sendMobileRPCResponse( Message(msgUnregistered, message.second) );
            RPC2Communication::OnAppUnregistered* appUnregistered = new RPC2Communication::OnAppUnregistered();
            appUnregistered->setAppName(appName);
            appUnregistered->setReason(AppInterfaceUnregisteredReason(AppInterfaceUnregisteredReason::USER_EXIT));
            sendHMIRPC2Response(appUnregistered);
            break;
        }
        case Marshaller::METHOD_SUBSCRIBEBUTTON_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A SubscribeButton request has been invoked");
            SubscribeButton_request * object = (SubscribeButton_request*)message.first;
            subscribeButton( message );
            SubscribeButton_response* response = new SubscribeButton_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(Result::SUCCESS);
            Message responseMessage = Message(response, sessionID);
            sendMobileRPCResponse( responseMessage );
            break;
        }
        case Marshaller::METHOD_UNSUBSCRIBEBUTTON_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An UnsubscribeButton request has been invoked");
            UnsubscribeButton_request * object = (UnsubscribeButton_request*)message.first;
            unsubscribeButton( message );
            UnsubscribeButton_response* response = new UnsubscribeButton_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(Result::SUCCESS);
            Message responseMessage = Message(response, sessionID);
            sendMobileRPCResponse( responseMessage );
            break;
        }
        case Marshaller::METHOD_SHOW_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A Show request has been invoked");
            LOG4CPLUS_INFO_EXT(mLogger, "message " << message.first );
            Show_request* object = (Show_request*)message.first;
            RPC2Communication::Show* showRPC2Request = new RPC2Communication::Show();
            LOG4CPLUS_INFO_EXT(mLogger, "showrpc2request created");
            if(object->get_mainField1())
            {
                showRPC2Request->setMainField1(*object->get_mainField1());
            }
            LOG4CPLUS_INFO_EXT(mLogger, "setMainField1 was called");
            if(object->get_mediaClock())
            {
                showRPC2Request->setMainField2(*object->get_mainField2());
            }
            if(object->get_mediaClock())
            {
                showRPC2Request->setMediaClock(*object->get_mediaClock());
            }
            if(object->get_statusBar())
            {
                showRPC2Request->setStatusBar(*object->get_statusBar());
            }
            if(object->get_alignment())
            {
                showRPC2Request->setTextAlignment(*object->get_alignment());
            }
            LOG4CPLUS_INFO_EXT(mLogger, "Show request almost handled" );
            mapMessageToSession(showRPC2Request->getID(), sessionID);
            sendHMIRPC2Response(showRPC2Request);
            break;
        }
        case Marshaller::METHOD_SPEAK_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A Speak request has been invoked");
            Speak_request* object = (Speak_request*)message.first;
            RPC2Communication::Speak* speakRPC2Request = new RPC2Communication::Speak();
            speakRPC2Request->setTTSChunks(object->get_ttsChunks());
            mapMessageToSession(speakRPC2Request->getID(), sessionID);
            sendHMIRPC2Response(speakRPC2Request);
            break;
        }
        case Marshaller::METHOD_SETGLOBALPROPERTIES_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A SetGlobalProperties request has been invoked");
            SetGlobalProperties_request* object = (SetGlobalProperties_request*)message.first;
            RPC2Communication::SetGlobalProperties* setGPRPC2Request = new RPC2Communication::SetGlobalProperties();
            mapMessageToSession(setGPRPC2Request->getID(), sessionID);
            if(object->get_helpPrompt())
            {
                setGPRPC2Request->setHelpPrompt(*object->get_helpPrompt());
            }

            if(object->get_timeoutPrompt())
            {
                setGPRPC2Request->setTimeoutPrompt(*object->get_timeoutPrompt());
            }
            sendHMIRPC2Response(setGPRPC2Request);
            break;
        }
        case Marshaller::METHOD_RESETGLOBALPROPERTIES_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A ResetGlobalProperties request has been invoked");
            ResetGlobalProperties_request* object = (ResetGlobalProperties_request*)message.first;
            RPC2Communication::ResetGlobalProperties* resetGPRPC2Request = new RPC2Communication::ResetGlobalProperties();
            mapMessageToSession(resetGPRPC2Request->getID(), sessionID);
            resetGPRPC2Request->setProperty(object->get_properties());

            sendHMIRPC2Response(resetGPRPC2Request);
            break;
        }
        case Marshaller::METHOD_SHOW_RESPONSE:
        case Marshaller::METHOD_SPEAK_RESPONSE:
        case Marshaller::METHOD_SETGLOBALPROPERTIES_RESPONSE:
        case Marshaller::METHOD_RESETGLOBALPROPERTIES_RESPONSE:
        case Marshaller::METHOD_REGISTERAPPINTERFACE_RESPONSE:
        case Marshaller::METHOD_SUBSCRIBEBUTTON_RESPONSE:
        case Marshaller::METHOD_UNSUBSCRIBEBUTTON_RESPONSE:
        case Marshaller::METHOD_ONAPPINTERFACEUNREGISTERED:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A "<< message.first->getMethodId() << " response or notification has been invoked");
            LOG4CPLUS_INFO_EXT(mLogger, "sendRPCMessage called for " << mJSONHandler << " message "<< message.first);
            mJSONHandler->sendRPCMessage(message.first, sessionID);
            break;
        }

		default:
            LOG4CPLUS_ERROR_EXT(mLogger, " An undefined RPC message "<< message.first->getMethodId() <<" has been received!");
			break;
    }
}

void AppMgrCore::handleMobileRPCNotification(ALRPCMessage *message)
{
    LOG4CPLUS_INFO_EXT(mLogger, " A mobile RPC notification "<< message->getMethodId() <<" has been received!");
    switch(message->getMethodId())
    {
        case Marshaller::METHOD_ONAPPINTERFACEUNREGISTERED:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnAppInterfaceUnregistered notification has been invoked");

            OnAppInterfaceUnregistered* object = (OnAppInterfaceUnregistered*)message;
            mJSONHandler->sendRPCMessage(message, 1);//just temporarily!!!
            break;
        }
        default:
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " An undefined RPC notification "<< message->getMethodId() <<" has been received!");
            break;
        }
    }
}

void AppMgrCore::handleBusRPCMessageIncoming( RPC2Communication::RPC2Command* msg )
{
    LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message "<< msg->getMethod() <<" has been incoming...");

	switch(msg->getMethod())
	{
        case RPC2Communication::RPC2Marshaller::METHOD_ONBUTTONEVENT:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnButtonEvent notification has been invoked");
            RPC2Communication::OnButtonEvent * object = (RPC2Communication::OnButtonEvent*)msg;
            OnButtonEvent* event = new OnButtonEvent();
            event->set_buttonEventMode(object->getMode());
            const ButtonName & name = object->getName();
            event->set_buttonName(name);
            unsigned char sessionID = findSessionIdSubscribedToButton(name);
            Message message = Message(event, sessionID);
            sendMobileRPCResponse( message );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_ONBUTTONPRESS:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnButtonPress notification has been invoked");
            RPC2Communication::OnButtonPress * object = (RPC2Communication::OnButtonPress*)msg;
            OnButtonPress* event = new OnButtonPress();
            const ButtonName & name = object->getName();
            event->set_buttonName(name);
            event->set_buttonPressMode(object->getMode());
            LOG4CPLUS_INFO_EXT(mLogger, "before we find sessionID");
            unsigned char sessionID = findSessionIdSubscribedToButton(name);
            LOG4CPLUS_INFO_EXT(mLogger, "sessionID found " << sessionID);
            Message message = Message(event, sessionID);
            sendMobileRPCResponse( message );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_GET_CAPABILITIES_RESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A GetButtonCapabilities response has been income");
            RPC2Communication::GetCapabilitiesResponse * object = (RPC2Communication::GetCapabilitiesResponse*)msg;
            setButtonCapabilities( object );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_SHOW_RESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A Show response has been income");
            RPC2Communication::ShowResponse* object = (RPC2Communication::ShowResponse*)msg;
            Show_response* response = new Show_response();
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_resultCode(object->getResult());
            response->set_success(true);
            unsigned char sessionID = findSessionIdByMessage(object->getID());
            removeMessageToSessionMapping(object->getID());
            Message responseMessage = Message(response, sessionID);
            sendMobileRPCResponse( responseMessage );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_SPEAK_RESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A Speak response has been income");
            RPC2Communication::SpeakResponse* object = (RPC2Communication::SpeakResponse*)msg;
            Speak_response* response = new Speak_response();
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_resultCode(object->getResult());
            response->set_success(true);
            unsigned char sessionID = findSessionIdByMessage(object->getID());
            removeMessageToSessionMapping(object->getID());
            Message responseMessage = Message(response, sessionID);
            sendMobileRPCResponse( responseMessage );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_SET_GLOBAL_PROPERTIES_RESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A SetGlobalProperties response has been income");
            SetGlobalProperties_response* response = new SetGlobalProperties_response();
            RPC2Communication::SetGlobalPropertiesResponse* object = (RPC2Communication::SetGlobalPropertiesResponse*)msg;
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_resultCode(object->getResult());
            response->set_success(true);
            unsigned char sessionID = findSessionIdByMessage(object->getID());
            removeMessageToSessionMapping(object->getID());
            Message responseMessage = Message(response, sessionID);
            sendMobileRPCResponse( responseMessage );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_RESET_GLOBAL_PROPERTIES_RESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A ResetGlobalProperties response has been income");
            ResetGlobalProperties_response* response = new ResetGlobalProperties_response();
            RPC2Communication::ResetGlobalPropertiesResponse* object = (RPC2Communication::ResetGlobalPropertiesResponse*)msg;
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(object->getResult());
            unsigned char sessionID = findSessionIdByMessage(object->getID());
            removeMessageToSessionMapping(object->getID());
            Message responseMessage = Message(response, sessionID);
            sendMobileRPCResponse( responseMessage );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_ONAPPUNREDISTERED:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnAppUnregistered notification has been income");
            RPC2Communication::OnAppUnregistered * object = (RPC2Communication::OnAppUnregistered*)msg;
            OnAppInterfaceUnregistered* event = new OnAppInterfaceUnregistered();
            event->set_reason(object->getReason());
            enqueueOutgoingMobileRPCNotification(event);
            break;
        }
		case RPC2Communication::RPC2Marshaller::METHOD_INVALID:
		default:
			LOG4CPLUS_ERROR_EXT(mLogger, " An undefined RPC message "<< msg->getMethod() <<" has been received!");

			break;
	}

    LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message "<< msg->getMethod() <<" has been invoked!");
}

void AppMgrCore::handleBusRPCMessageOutgoing(RPC2Communication::RPC2Command *msg)
{
    LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message "<< msg->getMethod() <<" has been outcoming...");
    switch(msg->getMethod())
    {
        case RPC2Communication::RPC2Marshaller::METHOD_GET_CAPABILITIES_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A GetCapabilities request has been outcoming");
            //RPC2Communication::GetCapabilities * object = (RPC2Communication::GetCapabilities*)msg;
            mJSONRPC2Handler -> sendRequest( static_cast<RPC2Communication::RPC2Request*>(msg) );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_SHOW_REQUEST:
        {
            mJSONRPC2Handler -> sendRequest( static_cast<RPC2Communication::RPC2Request*>(msg) );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_SPEAK_REQUEST:
        {
            mJSONRPC2Handler -> sendRequest( static_cast<RPC2Communication::RPC2Request*>(msg) );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_SET_GLOBAL_PROPERTIES_REQUEST:
        {
            mJSONRPC2Handler -> sendRequest( static_cast<RPC2Communication::RPC2Request*>(msg) );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_RESET_GLOBAL_PROPERTIES_REQUEST:
        {
            mJSONRPC2Handler -> sendRequest( static_cast<RPC2Communication::RPC2Request*>(msg) );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_ONAPPREGISTERED:
        {
            mJSONRPC2Handler -> sendNotification( static_cast<RPC2Communication::RPC2Notification*>(msg) );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_ONAPPUNREDISTERED:
        {
            mJSONRPC2Handler -> sendNotification( static_cast<RPC2Communication::RPC2Notification*>(msg) );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_INVALID:
        default:
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " An undefined RPC message "<< msg->getMethod() <<" has been received!");
            break;
        }
    }

    LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message "<< msg->getMethod() <<" has been outcomed");
}

void AppMgrCore::enqueueOutgoingMobileRPCMessage( const Message& message )
{
    LOG4CPLUS_INFO_EXT(mLogger, " A " << message.first->getMethodId() << " outgoing mobile RPC message send has been invoked");
	
	mMtxRPCAppLinkObjectsOutgoing.Lock();
	
	mQueueRPCAppLinkObjectsOutgoing.push(message);
	
	mMtxRPCAppLinkObjectsOutgoing.Unlock();
	
    LOG4CPLUS_INFO_EXT(mLogger, " A " << message.first->getMethodId() << " outgoing mobile RPC message has been sent");
}

void AppMgrCore::enqueueOutgoingMobileRPCNotification(ALRPCMessage *msg)
{
    LOG4CPLUS_INFO_EXT(mLogger, " A " << msg->getMethodId() << " outgoing mobile RPC notification send has been invoked");

    mMtxMobileRPCNotificationsOutgoing.Lock();

    mQueueMobileRPCNotificationsOutgoing.push(msg);

    mMtxMobileRPCNotificationsOutgoing.Unlock();

    LOG4CPLUS_INFO_EXT(mLogger, " A " << msg->getMethodId() << " outgoing mobile RPC notification has been sent");
}

void AppMgrCore::enqueueOutgoingBusRPCMessage( RPC2Communication::RPC2Command * message )
{
	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getMethod() << " outgoing mobile RPC message send has been invoked");
	
	mMtxRPCBusObjectsOutgoing.Lock();
	
	mQueueRPCBusObjectsOutgoing.push(message);
	
	mMtxRPCBusObjectsOutgoing.Unlock();

    LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getMethod() << " outgoing mobile RPC message has been sent");
}

void AppMgrCore::mapMessageToSession(int messageId, unsigned char sessionId)
{
    mMessagesToSessionsMap.insert(MessageToSession(messageId, sessionId));
}

void AppMgrCore::removeMessageToSessionMapping(int messageId)
{
    mMessagesToSessionsMap.erase(messageId);
}

unsigned char AppMgrCore::findSessionIdSubscribedToButton( ButtonName appName ) const
{
    ButtonMap::const_iterator it = mButtonsMapping.find( appName );
    if ( it != mButtonsMapping.end() )
    {
        if ( !it-> second )
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "RegistryItem not found" );
            return '0';
        }
        if ( it->second->getApplication() )
        {
            return it->second->getApplication()->getSessionID();
        }
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Button " << appName.get() << " not found in subscribed." );
    return '0';
}

unsigned char AppMgrCore::findSessionIdByMessage(int messageId) const
{
    return mMessagesToSessionsMap.find(messageId)->second;
}

const RegistryItem* AppMgrCore::registerApplication( const Message& object )
{
    ALRPCMessage* msg = object.first;
    unsigned char sessionID = object.second;
    RegisterAppInterface_request * request = (RegisterAppInterface_request*)msg;
    LOG4CPLUS_INFO_EXT(mLogger, " Registering an application " << request->get_appName() << "!");

    const std::string& appName = request->get_appName();
    Application* application = new Application( appName, sessionID );

    bool isMediaApplication = request->get_isMediaApplication();
    const Language& languageDesired = request->get_languageDesired();
    const SyncMsgVersion& syncMsgVersion = request->get_syncMsgVersion();

    if ( request -> get_ngnMediaScreenAppName() )
    {
        const std::string& ngnMediaScreenAppName = *request->get_ngnMediaScreenAppName();
        application->setNgnMediaScreenAppName(ngnMediaScreenAppName);            
    }
    
    if ( request -> get_vrSynonyms() )
    {
        const std::vector<std::string>& vrSynonyms = *request->get_vrSynonyms();
        application->setVrSynonyms(vrSynonyms);            
    }

    if ( request -> get_usesVehicleData() )
    {
        bool usesVehicleData = request->get_usesVehicleData();
        application->setUsesVehicleData(usesVehicleData);            
    }
    
    if ( request-> get_autoActivateID() )
    {
        const std::string& autoActivateID = *request->get_autoActivateID();
        application->setAutoActivateID(autoActivateID);
    }    

	application->setIsMediaApplication(isMediaApplication);
	application->setLanguageDesired(languageDesired);
	application->setSyncMsgVersion(syncMsgVersion);

	application->setApplicationHMIStatusLevel(HMILevel::HMI_NONE);
    LOG4CPLUS_INFO_EXT(mLogger, "Application created." );

    return AppMgrRegistry::getInstance().registerApplication( application );
}

void AppMgrCore::unregisterApplication(const Message &msg)
{
    ALRPCMessage* message = msg.first;
    unsigned char sessionID = msg.second;
    UnregisterAppInterface_request* request = (UnregisterAppInterface_request*)message;
    RegistryItem* app = AppMgrRegistry::getInstance().getItem(sessionID);
    const std::string& appName = app->getApplication()->getName();
    LOG4CPLUS_INFO_EXT(mLogger, " Unregistering an application " << appName << "!");
    clearButtonSubscribtion(sessionID);
    AppMgrRegistry::getInstance().unregisterApplication(app);

    LOG4CPLUS_INFO_EXT(mLogger, " Unregistered an application " << appName << "!");
}

void AppMgrCore::subscribeButton( const Message& msg )
{
    ALRPCMessage* message = msg.first;
    unsigned char sessionID = msg.second;
    SubscribeButton_request * object = (SubscribeButton_request*)message;
    const ButtonName& name = object->get_buttonName();
    RegistryItem* item = AppMgrRegistry::getInstance().getItem(sessionID);
    LOG4CPLUS_INFO_EXT(mLogger, "Subscribe to button " << name.get() << " in session " << sessionID );
    mButtonsMapping.insert(ButtonMapItem(name, item));
}

void AppMgrCore::unsubscribeButton(const Message& msg )
{
    ALRPCMessage* message = msg.first;
    unsigned char sessionID = msg.second;
    UnsubscribeButton_request * object = (UnsubscribeButton_request*)message;
    const ButtonName& name = object->get_buttonName();
    mButtonsMapping.erase(name);
}

void AppMgrCore::clearButtonSubscribtion(unsigned char sessionID)
{
    for(ButtonMap::iterator it = mButtonsMapping.begin(); it != mButtonsMapping.end(); it++)
    {
        if(it->second->getApplication()->getSessionID() == sessionID)
        {
            mButtonsMapping.erase(it->first);
        }
    }
}

const ALRPCMessage* AppMgrCore::queryInfoForRegistration( const RegistryItem* registryItem )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Querying info for registration of an application " << registryItem->getApplication()->getName() << "!");

 //   RPC2Communication::OnButton

	LOG4CPLUS_INFO_EXT(mLogger, " Queried info for registration of an application " << registryItem->getApplication()->getName() << "!");
}

void AppMgrCore::registerApplicationOnHMI( const std::string& name )
{

}

void AppMgrCore::setButtonCapabilities( RPC2Communication::GetCapabilitiesResponse* msg )
{
    std::vector<RPC2Communication::GetCapabilitiesResponse::GetCapabilitiesResponseInternal> result = msg->getCapabilities();
    Capabilities caps;
    for( std::vector<RPC2Communication::GetCapabilitiesResponse::GetCapabilitiesResponseInternal>::iterator it = result.begin(); it != result.end(); it++ )
    {
        mButtonCapabilities.push_back(it->capability);
    }
}

const Capabilities& AppMgrCore::getButtonCapabilities() const
{
	return mButtonCapabilities;
}

void AppMgrCore::sendMobileRPCResponse( const Message& msg )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Sending mobile RPC response to "<< msg.first->getMethodId() <<"!");

    enqueueOutgoingMobileRPCMessage( msg );
}

void AppMgrCore::sendMobileRPCNotification(ALRPCMessage *msg)
{
    LOG4CPLUS_INFO_EXT(mLogger, " Sending mobile RPC notification to "<< msg->getMethodId() <<"!");

}

void AppMgrCore::sendHMIRPC2Response(RPC2Communication::RPC2Command *msg)
{
    LOG4CPLUS_INFO_EXT(mLogger, " Sending HMI RPC2 response to "<< msg->getMethod() <<"!");

    enqueueOutgoingBusRPCMessage(msg);
}

void* AppMgrCore::handleQueueRPCBusObjectsIncoming( void* )
{
	while(true)
	{
		std::size_t size = mQueueRPCBusObjectsIncoming.size();
		if( size > 0 )
		{
			mMtxRPCBusObjectsIncoming.Lock();
			RPC2Communication::RPC2Command* msg = mQueueRPCBusObjectsIncoming.front();
			mQueueRPCBusObjectsIncoming.pop();
			mMtxRPCBusObjectsIncoming.Unlock();
			if(!msg)
			{
				LOG4CPLUS_ERROR_EXT(mLogger, " Erroneous null-message has been received!");
				continue;
			}
			
			handleBusRPCMessageIncoming( msg );
		}
	}
}

void* AppMgrCore::handleQueueRPCAppLinkObjectsOutgoing( void* )
{
	while(true)
	{
		std::size_t size = mQueueRPCAppLinkObjectsOutgoing.size();
		if( size > 0 )
		{
			mMtxRPCAppLinkObjectsOutgoing.Lock();
            Message msg = mQueueRPCAppLinkObjectsOutgoing.front();
			mQueueRPCAppLinkObjectsOutgoing.pop();
			mMtxRPCAppLinkObjectsOutgoing.Unlock();
            if(!msg.first)
			{
				LOG4CPLUS_ERROR_EXT(mLogger, " Erroneous null-message has been received!");
				continue;
			}
			
			handleMobileRPCMessage( msg );
		}
	}
}

void* AppMgrCore::handleQueueRPCAppLinkObjectsIncoming( void* )
{
	while(true)
	{
		std::size_t size = mQueueRPCAppLinkObjectsIncoming.size();
		if( size > 0 )
		{
			mMtxRPCAppLinkObjectsIncoming.Lock();
            Message message = mQueueRPCAppLinkObjectsIncoming.front();

			mQueueRPCAppLinkObjectsIncoming.pop();
			mMtxRPCAppLinkObjectsIncoming.Unlock();
            if(!message.first)
			{
				LOG4CPLUS_ERROR_EXT(mLogger, " Erroneous null-message has been received!");
				continue;
			}
			
            handleMobileRPCMessage( message );
		}
	}
}

void* AppMgrCore::handleQueueRPCBusObjectsOutgoing( void* )
{
	while(true)
	{
		std::size_t size = mQueueRPCBusObjectsOutgoing.size();
		if( size > 0 )
		{
			mMtxRPCBusObjectsOutgoing.Lock();
			RPC2Communication::RPC2Command* msg = mQueueRPCBusObjectsOutgoing.front();
			mQueueRPCBusObjectsOutgoing.pop();
			mMtxRPCBusObjectsOutgoing.Unlock();
			if(!msg)
			{
				LOG4CPLUS_ERROR_EXT(mLogger, " Erroneous null-message has been received!");
				continue;
			}
			
            handleBusRPCMessageOutgoing( msg );
		}
    }
}

void *AppMgrCore::handleQueueMobileRPCNotificationsOutgoing(void *)
{
    while(true)
    {
        std::size_t size = mQueueMobileRPCNotificationsOutgoing.size();
        if( size > 0 )
        {
            mMtxMobileRPCNotificationsOutgoing.Lock();
            ALRPCMessage* msg = mQueueMobileRPCNotificationsOutgoing.front();
            mQueueMobileRPCNotificationsOutgoing.pop();
            mMtxMobileRPCNotificationsOutgoing.Unlock();
            if(!msg)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " Erroneous null-message has been received!");
                continue;
            }

            handleMobileRPCNotification( msg );
        }
    }
}

void AppMgrCore::setJsonHandler(JSONHandler* handler)
{
	mJSONHandler = handler;
}

JSONHandler* AppMgrCore::getJsonHandler( ) const
{
    return mJSONHandler;
}

void AppMgrCore::setJsonRPC2Handler(JSONRPC2Handler *handler)
{
    mJSONRPC2Handler = handler;
}

JSONRPC2Handler *AppMgrCore::getJsonRPC2Handler() const
{
    return mJSONRPC2Handler;
}

bool Comparer::operator ()(const ButtonName &b1, const ButtonName &b2) const
{
    return b1.get() < b2.get();
}

};
