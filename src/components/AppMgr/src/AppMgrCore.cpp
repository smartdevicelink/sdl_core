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
}

void AppMgrCore::pushMobileRPCMessage( MobileRPCMessage * message )
{
	mMtxRPCAppLinkObjectsIncoming.Lock();
	
	mQueueRPCAppLinkObjectsIncoming.push((RPCAppLinkObject *)message);
	
	mMtxRPCAppLinkObjectsIncoming.Unlock();
}

void AppMgrCore::executeThreads()
{
	mThreadRPCAppLinkObjectsIncoming.Start(false);
	mThreadRPCAppLinkObjectsOutgoing.Start(false);
	mThreadRPCBusObjectsIncoming.Start(false);
	mThreadRPCBusObjectsOutgoing.Start(false);
	
	while(!m_bTerminate)
	{
	}
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
				//to log an error: invalid object
				continue;
			}

			handleMobileRPCMessage( msg );
		}
	}
}

void AppMgrCore::handleMobileRPCMessage( MobileRPCMessage* msg )
{
	switch(msg->getMessageType())
	{
		case MobileRPCMessage::REQUEST:
		{
			if(0 == msg->getFunctionName().compare("RegisterAppInterface"))
			{
				RegisterAppInterface * object = (RegisterAppInterface*)msg;
				registerApplication( object );
				sendMobileRPCResponse( msg );
			}
			else if(0 == msg->getFunctionName().compare("SubscribeButton"))
			{
			//	SubscribeButton * object = (SubscribeButton*)msg;
			//	registerApplication( object );
				sendMobileRPCResponse( msg );
			}
			break;
		}
		case MobileRPCMessage::RESPONSE:
		case MobileRPCMessage::NOTIFICATION:
		{
			mJSONHandler->sendRPCMessage(msg);
			break;
		}
		case MobileRPCMessage::UNDEFINED:
		default:
			//unknown RPC message - notifying about an error
			break;
	}
}

void AppMgrCore::handleBusRPCMessage( RPCBusObject* msg )
{
	//right now handles only outgoing messages
	//assumes that the message in param is outgoing
	AppLinkInterface::getInstance().sendRPCBusObject( msg );
}

void AppMgrCore::enqueueOutgoingMobileRPCMessage( MobileRPCMessage * message )
{
	mMtxRPCAppLinkObjectsOutgoing.Lock();
	
	mQueueRPCAppLinkObjectsOutgoing.push((RPCAppLinkObject *)message);
	
	mMtxRPCAppLinkObjectsOutgoing.Unlock();
}

void AppMgrCore::enqueueOutgoingBusRPCMessage( RPCBusObject * message )
{
	mMtxRPCBusObjectsOutgoing.Lock();
	
	mQueueRPCBusObjectsOutgoing.push(message);
	
	mMtxRPCBusObjectsOutgoing.Unlock();
}

void AppMgrCore::registerApplication( RegisterAppInterface* object )
{
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

	AppMgrRegistry::getInstance().registerApplication( application );
}

void AppMgrCore::sendMobileRPCResponse( MobileRPCMessage* msg )
{
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
				//to log an error: invalid object
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
			RPCBusObject* msg = mQueueRPCBusObjectsOutgoing.front();
			mQueueRPCBusObjectsOutgoing.pop();
			mMtxRPCBusObjectsOutgoing.Unlock();
			if(!msg)
			{
				//to log an error: invalid object
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
