/*
 * AppMgr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrCore.h"
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
#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/ALRPCObjects/HMILevel.h"
#include "AppMgr/IApplication.h"
#include "AppMgr/Application.h"
#include "AppMgr/AppMgrRegistry.h"
#include "AppMgr/AppPolicy.h"
#include "AppMgr/RegistryItem.h"
#include "AppMgr/AppLinkInterface.h"
#include <sys/socket.h>
#include "LoggerHelper.hpp"
#include "JSONHandler/OnButtonEvent.h"
#include "JSONHandler/RPC2Marshaller.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/ALRPCObjects/ButtonCapabilities.h"
#include "JSONHandler/GetCapabilitiesResponse.h"
#include "JSONHandler/ALRPCObjects/AppInterfaceUnregisteredReason.h"
#include "JSONHandler/ALRPCObjects/OnButtonEvent.h"
#include "JSONHandler/ALRPCObjects/OnButtonPress.h"

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
	,m_bTerminate(false)
{
    LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore constructed!");
}

AppMgrCore::AppMgrCore(const AppMgrCore &)
    :mThreadRPCAppLinkObjectsIncoming(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCAppLinkObjectsIncoming, NULL))
    ,mThreadRPCAppLinkObjectsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCAppLinkObjectsOutgoing, NULL))
    ,mThreadRPCBusObjectsIncoming(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCBusObjectsIncoming, NULL))
    ,mThreadRPCBusObjectsOutgoing(new System::ThreadArgImpl<AppMgrCore>(*this, &AppMgrCore::handleQueueRPCBusObjectsOutgoing, NULL))
    ,m_bTerminate(false)
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

void AppMgrCore::pushMobileRPCMessage( ALRPCMessage * message )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Pushing mobile RPC message...");
	mMtxRPCAppLinkObjectsIncoming.Lock();
	
	mQueueRPCAppLinkObjectsIncoming.push(message);
	
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

	LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
}

void AppMgrCore::terminateThreads()
{
	m_bTerminate = true;
}

void AppMgrCore::handleMobileRPCMessage( ALRPCMessage* msg )
{
	LOG4CPLUS_INFO_EXT(mLogger, " A mobile RPC message "<< msg->getMethodId() <<" has been received!");
	switch(msg->getMethodId())
	{
		case Marshaller::METHOD_REGISTERAPPINTERFACE_REQUEST:
		{
			LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface request has been invoked");
			RegisterAppInterface_request * object = (RegisterAppInterface_request*)msg;
			const RegistryItem* registeredApp =  registerApplication( object );
            ALRPCMessage* info = queryInfoForRegistration( registeredApp );
            RegisterAppInterface_response* response = new RegisterAppInterface_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_autoActivateID(*object->get_autoActivateID());
            response->set_buttonCapabilities(getButtonCapabilities());
            response->set_success(true);
            response->set_resultCode(Result::SUCCESS);
            sendMobileRPCResponse( response );
            break;
		}
        case Marshaller::METHOD_SUBSCRIBEBUTTON_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A SubscribeButton request has been invoked");
            SubscribeButton_request * object = (SubscribeButton_request*)msg;
            subscribeButton( object );
            SubscribeButton_response* response = new SubscribeButton_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(Result::SUCCESS);
            sendMobileRPCResponse( response );
            break;
        }
        case Marshaller::METHOD_UNSUBSCRIBEBUTTON_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An UnsubscribeButton request has been invoked");
            UnsubscribeButton_request * object = (UnsubscribeButton_request*)msg;
            unsubscribeButton( object );
            UnsubscribeButton_response* response = new UnsubscribeButton_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(Result::SUCCESS);
            sendMobileRPCResponse( response );
            break;
        }
        case Marshaller::METHOD_REGISTERAPPINTERFACE_RESPONSE:
        case Marshaller::METHOD_SUBSCRIBEBUTTON_RESPONSE:
        case Marshaller::METHOD_UNSUBSCRIBEBUTTON_RESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A "<< msg->getMethodId() << " response or notification has been invoked");
            mJSONHandler->sendRPCMessage(msg);
            break;
        }

		default:
			LOG4CPLUS_ERROR_EXT(mLogger, " An undefined RPC message "<< msg->getMethodId() <<" has been received!");
			break;
	}
}

void AppMgrCore::handleBusRPCMessageIncoming( RPC2Communication::RPC2Command* msg )
{
	LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message "<< msg->getMethod() <<" has been invoking...");

	switch(msg->getMethod())
	{
		case RPC2Communication::RPC2Marshaller::METHOD_GET_CAPABILITIES_REQUEST:
		{			
			LOG4CPLUS_INFO_EXT(mLogger, " A GetCapabilitiesResponse request has been invoked");
			RPC2Communication::GetCapabilitiesResponse * object = (RPC2Communication::GetCapabilitiesResponse*)msg;
			setButtonCapabilities( object );
			
			break;
		}
        case RPC2Communication::RPC2Marshaller::METHOD_ONBUTTONEVENT:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnButtonEvent request has been invoked");
            RPC2Communication::OnButtonEvent * object = (RPC2Communication::OnButtonEvent*)msg;
            OnButtonEvent* event = new OnButtonEvent();
            event->set_buttonEventMode(object->getMode());
            event->set_buttonName(object->getName());
            sendMobileRPCResponse( event );
            break;
        }
        case RPC2Communication::RPC2Marshaller::METHOD_ONBUTTONPRESS:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnButtonPress request has been invoked");
            RPC2Communication::OnButtonPress * object = (RPC2Communication::OnButtonPress*)msg;
            OnButtonPress* event = new OnButtonPress();
            event->set_buttonName(object->getName());
            event->set_buttonPressMode(object->getMode());
            sendMobileRPCResponse( event );
            break;
        }
		case RPC2Communication::RPC2Marshaller::METHOD_INVALID:
		default:
			LOG4CPLUS_ERROR_EXT(mLogger, " An undefined RPC message "<< msg->getMethod() <<" has been received!");
			break;
	}
//	AppLinkInterface::getInstance().sendRPCCommand( msg );
	LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message "<< msg->getMethod() <<" has been invoked!");
}

void AppMgrCore::enqueueOutgoingMobileRPCMessage( ALRPCMessage * message )
{
	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getMethodId() << " outgoing mobile RPC message send has been invoked");
	
	mMtxRPCAppLinkObjectsOutgoing.Lock();
	
	mQueueRPCAppLinkObjectsOutgoing.push(message);
	
	mMtxRPCAppLinkObjectsOutgoing.Unlock();
	
	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getMethodId() << " outgoing mobile RPC message has been sent");
}

void AppMgrCore::enqueueOutgoingBusRPCMessage( RPC2Communication::RPC2Command * message )
{
	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getMethod() << " outgoing mobile RPC message send has been invoked");
	
	mMtxRPCBusObjectsOutgoing.Lock();
	
	mQueueRPCBusObjectsOutgoing.push(message);
	
	mMtxRPCBusObjectsOutgoing.Unlock();

	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getMethod() << " outgoing mobile RPC message has been sent");
}

const RegistryItem* AppMgrCore::registerApplication( RegisterAppInterface_request* object )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Registering an application " << object->get_appName() << "!");
	const std::string& appName = object->get_appName();
	//const std::string& ngnMediaScreenAppName = object->get_ngnMediaScreenAppName();
	//const std::vector<std::string>& vrSynonyms = object->get_vrSynonyms();
	bool usesVehicleData = object->get_usesVehicleData();
	bool isMediaApplication = object->get_isMediaApplication();
	const Language& languageDesired = object->get_languageDesired();
	//const std::string& autoActivateID = object->get_autoActivateID();
	const SyncMsgVersion& syncMsgVersion = object->get_syncMsgVersion();

	Application* application = new Application( appName );
	//application->setAutoActivateID(autoActivateID);
	application->setIsMediaApplication(isMediaApplication);
	application->setLanguageDesired(languageDesired);
	//application->setNgnMediaScreenAppName(ngnMediaScreenAppName);
	application->setSyncMsgVersion(syncMsgVersion);
	application->setUsesVehicleData(usesVehicleData);
	//application->setVrSynonyms(vrSynonyms);

	application->setApplicationHMIStatusLevel(HMILevel::HMI_NONE);

//    RPC2Communication::

	return AppMgrRegistry::getInstance().registerApplication( application );
}

void AppMgrCore::subscribeButton( SubscribeButton_request* msg )
{

}

void AppMgrCore::unsubscribeButton(UnsubscribeButton_request *msg )
{

}

ALRPCMessage* AppMgrCore::queryInfoForRegistration( const RegistryItem* registryItem )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Querying info for registration of an application " << registryItem->getApplication()->getName() << "!");

	

	LOG4CPLUS_INFO_EXT(mLogger, " Queried info for registration of an application " << registryItem->getApplication()->getName() << "!");
}

void AppMgrCore::registerApplicationOnHMI( const std::string& name )
{
//	RPCBusObject* object = new RPCBusObject( 1, RPCBusObject::REQUEST, "AppLinkCore.OnAppRegister" );
//	object->setParameter("name", name);
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

Capabilities AppMgrCore::getButtonCapabilities() const
{
	return mButtonCapabilities;
}

void AppMgrCore::sendMobileRPCResponse( ALRPCMessage* msg )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Sending mobile RPC response to "<< msg->getMethodId() <<"!");

    enqueueOutgoingMobileRPCMessage( msg );
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
			ALRPCMessage* msg = mQueueRPCAppLinkObjectsOutgoing.front();
			mQueueRPCAppLinkObjectsOutgoing.pop();
			mMtxRPCAppLinkObjectsOutgoing.Unlock();
			if(!msg)
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
			ALRPCMessage* msg = mQueueRPCAppLinkObjectsIncoming.front();
			mQueueRPCAppLinkObjectsIncoming.pop();
			mMtxRPCAppLinkObjectsIncoming.Unlock();
			if(!msg)
			{
				LOG4CPLUS_ERROR_EXT(mLogger, " Erroneous null-message has been received!");
				continue;
			}
			
			handleMobileRPCMessage( msg );
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
			
			handleBusRPCMessageIncoming( msg );
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

};
