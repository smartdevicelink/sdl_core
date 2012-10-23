/*
 * AppMgr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrCore.h"
#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/MobileRPCRequest.h"
#include "JSONHandler/MobileRPCResponse.h"
#include "JSONHandler/MobileRPCNotification.h"
#include "JSONHandler/MobileRPCFactory.h"
#include "JSONHandler/RegisterAppInterface.h"
#include "JSONHandler/RegisterAppInterfaceResponse.h"
#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/HMILevel.h"
#include "AppMgr/IApplication.h"
#include "AppMgr/Application.h"
#include "AppMgr/AppMgrRegistry.h"
#include "AppMgr/AppPolicy.h"
#include "AppMgr/RPCAppLinkObject.h"
#include "AppMgr/RPCBusObject.h"
#include "AppMgr/AppPolicy.h"
#include "AppMgr/RegistryItem.h"
#include "AppMgr/AppLinkInterface.h"
#include <sys/socket.h>
#include "AppMgr/AppMgrRegistry.h"
#include "LoggerHelper.hpp"
#include "JSONHandler/OnButtonEvent.h"

namespace NsAppManager
{
	
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
	,mLogger( log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore")) )
{
	LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore constructed!");
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

void AppMgrCore::pushMobileRPCMessage( MobileRPCMessage * message )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Pushing mobile RPC message...");
	mMtxRPCAppLinkObjectsIncoming.Lock();
	
	mQueueRPCAppLinkObjectsIncoming.push((RPCAppLinkObject *)message);
	
	mMtxRPCAppLinkObjectsIncoming.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Pushed mobile RPC message");
}

void AppMgrCore::executeThreads()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");
	mThreadRPCAppLinkObjectsIncoming.Start(false);
	mThreadRPCAppLinkObjectsOutgoing.Start(false);
	mThreadRPCBusObjectsIncoming.Start(false);
	mThreadRPCBusObjectsOutgoing.Start(false);

	LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
	
	while(!m_bTerminate)
	{
	}

	LOG4CPLUS_INFO_EXT(mLogger, " Threads are being stopped!");
}

void AppMgrCore::terminateThreads()
{
	m_bTerminate = true;
}

void* AppMgrCore::handleQueueRPCAppLinkObjectsIncoming( void* )
{
	while(true)
	{
		std::size_t size = mQueueRPCAppLinkObjectsIncoming.size();
		if( size > 0 )
		{
			mMtxRPCAppLinkObjectsIncoming.Lock();
			MobileRPCMessage* msg = mQueueRPCAppLinkObjectsIncoming.front();
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

void AppMgrCore::handleMobileRPCMessage( MobileRPCMessage* msg )
{
	LOG4CPLUS_INFO_EXT(mLogger, " A mobile RPC message "<< msg->getFunctionName() <<" has been received!");
	switch(msg->getMessageType())
	{
		case MobileRPCMessage::REQUEST:
		{
			if(0 == msg->getFunctionName().compare("RegisterAppInterface"))
			{
				LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface request has been invoked");
				RegisterAppInterface * object = (RegisterAppInterface*)msg;
				const RegistryItem* registeredApp =  registerApplication( object );
				MobileRPCMessage* response = queryInfoForRegistration( registeredApp );
				sendMobileRPCResponse( response );
			}
			else if(0 == msg->getFunctionName().compare("SubscribeButton"))
			{
				LOG4CPLUS_INFO_EXT(mLogger, " A SubscribeButton request has been invoked");
			//	SubscribeButton * object = (SubscribeButton*)msg;
			//	registerApplication( object );
				sendMobileRPCResponse( msg );
			}
			break;
		}
		case MobileRPCMessage::RESPONSE:
		case MobileRPCMessage::NOTIFICATION:
		{
			LOG4CPLUS_INFO_EXT(mLogger, " A "<< msg->getFunctionName() << " response or notification has been invoked");
			mJSONHandler->sendRPCMessage(msg);
			break;
		}
		case MobileRPCMessage::UNDEFINED:
		default:
			LOG4CPLUS_ERROR_EXT(mLogger, " An undefined RPC message "<< msg->getFunctionName() <<" has been received!");
			break;
	}
}

void AppMgrCore::handleBusRPCMessage( RPC2Communication::RPC2Command* msg )
{
	//right now handles only outgoing messages
	//assumes that the message in param is outgoing
	LOG4CPLUS_INFO_EXT(mLogger, " A " << msg->getMethod() << " bus message request has been invoked");
	AppLinkInterface::getInstance().sendRPCCommand( msg );
}

void AppMgrCore::enqueueOutgoingMobileRPCMessage( MobileRPCMessage * message )
{
	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getFunctionName() << " outgoing mobile RPC message send has been invoked");
	
	mMtxRPCAppLinkObjectsOutgoing.Lock();
	
	mQueueRPCAppLinkObjectsOutgoing.push((RPCAppLinkObject *)message);
	
	mMtxRPCAppLinkObjectsOutgoing.Unlock();
	
	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getFunctionName() << " outgoing mobile RPC message has been sent");
}

void AppMgrCore::enqueueOutgoingBusRPCMessage( RPC2Communication::RPC2Command * message )
{
	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getMethod() << " outgoing mobile RPC message send has been invoked");
	
	mMtxRPCBusObjectsOutgoing.Lock();
	
	mQueueRPCBusObjectsOutgoing.push(message);
	
	mMtxRPCBusObjectsOutgoing.Unlock();

	LOG4CPLUS_INFO_EXT(mLogger, " A " << message->getMethod() << " outgoing mobile RPC message has been sent");
}

const RegistryItem* AppMgrCore::registerApplication( RegisterAppInterface* object )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Registering an application " << object->getAppName() << "!");
	const std::string& appName = object->getAppName();
	const std::string& ngnMediaScreenAppName = object->getNgnMediaScreenAppName();
	const std::vector<std::string>& vrSynonyms = object->getVrSynonyms();
	bool usesVehicleData = object->getUsesVehicleData();
	bool isMediaApplication = object->getIsMediaApplication();
	const Language& languageDesired = object->getLanguageDesired();
	const std::string& autoActivateID = object->getAutoActivateID();
	const SyncMsgVersion& syncMsgVersion = object->getSyncMsgVersion();

	Application* application = new Application( appName );
	application->setAutoActivateID(autoActivateID);
	application->setIsMediaApplication(isMediaApplication);
	application->setLanguageDesired(languageDesired);
	application->setNgnMediaScreenAppName(ngnMediaScreenAppName);
	application->setSyncMsgVersion(syncMsgVersion);
	application->setUsesVehicleData(usesVehicleData);
	application->setVrSynonyms(vrSynonyms);

	application->setApplicationHMIStatusLevel(HMILevel::NONE);

	return AppMgrRegistry::getInstance().registerApplication( application );
}

MobileRPCMessage* AppMgrCore::queryInfoForRegistration( const RegistryItem* registryItem )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Querying info for registration of an application " << registryItem->getApplication()->getName() << "!");
}

void AppMgrCore::registerApplicationOnHMI( const std::string& name )
{
//	RPCBusObject* object = new RPCBusObject( 1, RPCBusObject::REQUEST, "AppLinkCore.OnAppRegister" );
//	object->setParameter("name", name);
}

void AppMgrCore::sendMobileRPCResponse( MobileRPCMessage* msg )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Sending mobile RPC response to "<< msg->getFunctionName() <<"!");
	MobileRPCFactory factory;
	RegisterAppInterfaceResponse* response = factory.createRegisterAppInterfaceResponse( *msg );
	enqueueOutgoingMobileRPCMessage( response );
}

void* AppMgrCore::handleQueueRPCBusObjectsIncoming( void* )
{
}

void* AppMgrCore::handleQueueRPCAppLinkObjectsOutgoing( void* )
{
	while(true)
	{
		std::size_t size = mQueueRPCAppLinkObjectsOutgoing.size();
		if( size > 0 )
		{
			mMtxRPCAppLinkObjectsOutgoing.Lock();
			MobileRPCMessage* msg = mQueueRPCAppLinkObjectsOutgoing.front();
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
			
			handleBusRPCMessage( msg );
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
