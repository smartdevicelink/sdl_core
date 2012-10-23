/*
 * AppLinkInterface.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgr.h"
#include "AppMgr/AppLinkInterface.h"
#include "AppMgr/AppMgrCore.h"
#include "CMessageBroker.hpp"
#include "CMessageBrokerRegistry.hpp"
#include "CSender.hpp"
#include <map>
#include <future>
#include "LoggerHelper.hpp"

namespace NsAppManager
{

std::string AppLinkInterface::mAddress = "";
uint16_t AppLinkInterface::mPort = 0;
std::string AppLinkInterface::mName = "";
bool AppLinkInterface::m_bInitialized = false;

AppLinkInterface::AppLinkInterface( const std::string& address, uint16_t port, const std::string& name )
	:NsMessageBroker::CMessageBrokerController::CMessageBrokerController(address, port, name)
	,mLogger( log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppLinkInterface")) )
	,mThreadRPCBusObjectsIncoming(new System::ThreadArgImpl<AppLinkInterface>(*this, &AppLinkInterface::handleQueueRPCBusObjectsIncoming, NULL))
	,mThreadRPCBusObjectsOutgoing(new System::ThreadArgImpl<AppLinkInterface>(*this, &AppLinkInterface::handleQueueRPCBusObjectsOutgoing, NULL))
	,m_bTerminate(false)
	,m_bButtonCapsRetrieved(false)
	,m_bVRCapsRetrieved(false)
	,m_bVoiceCapsRetrieved(false)
{
	LOG4CPLUS_INFO_EXT(mLogger, " AppLinkInterface constructed!");
}

AppLinkInterface::~AppLinkInterface( )
{
	LOG4CPLUS_INFO_EXT(mLogger, " AppLinkInterface constructed!");
}

AppLinkInterface& AppLinkInterface::getInstance( )
{
	if(m_bInitialized)
	{
		static AppLinkInterface instance(mAddress, mPort, mName);
		return instance;
	}
	//here to log error
}

void AppLinkInterface::setParams(const std::string& address, uint16_t port, std::string name)
{
	mAddress = address;
	mPort = port;
	mName = name;
	m_bInitialized = true;
}

void AppLinkInterface::sendRPCCommand( const RPC2Communication::RPC2Command* rpcObject )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Sending RPC Bus object "<< rpcObject->getMethod() <<" to HMI...");

	Json::Value value = RPC2Communication::RPC2Marshaller::toJSON(rpcObject);
	sendJsonMessage(value);
	
	LOG4CPLUS_INFO_EXT(mLogger, " Sent RPC Bus object "<< rpcObject->getMethod() <<" to HMI");
}

void AppLinkInterface::enqueueRPCCommandIncoming( RPC2Communication::RPC2Command * object )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Pushing RPC incoming bus message...");
	mMtxRPCBusObjectsIncoming.Lock();
	
	mQueueRPCBusObjectsIncoming.push(object);
	
	mMtxRPCBusObjectsIncoming.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Pushed RPC incoming bus message");
}

void AppLinkInterface::enqueueRPCCommandOutgoing( RPC2Communication::RPC2Command * object )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Pushing RPC outgoing bus message...");
	mMtxRPCBusObjectsOutgoing.Lock();
	
	mQueueRPCBusObjectsOutgoing.push(object);
	
	mMtxRPCBusObjectsOutgoing.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Pushed RPC outgoing bus message");
}

void AppLinkInterface::addAwaitedResponseMethod( int method )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Adding awaited response method "<<method);
	mMtxAwaitedResponseMethods.Lock();
	mAwaitedResponseMethods.insert(method);
	mMtxAwaitedResponseMethods.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Added awaited response method "<<method);
}

void AppLinkInterface::addRespondedMethod( int method )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Adding responded method "<<method);
	mMtxRespondedMethods.Lock();
	mRespondedMethods.insert(method);
	mMtxRespondedMethods.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Added responded method "<<method);
}

void AppLinkInterface::removeAwaitedResponseMethod( int method )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Adding awaited response method "<<method);
	mMtxAwaitedResponseMethods.Lock();
	std::set<int>::iterator it = mAwaitedResponseMethods.find(method);
	mAwaitedResponseMethods.erase(it);
	mMtxAwaitedResponseMethods.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Added awaited response method "<<method);
}

void AppLinkInterface::removeRespondedMethod( int method )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Adding responded method "<<method);
	mMtxRespondedMethods.Lock();
	std::set<int>::iterator it = mRespondedMethods.find(method);
	mRespondedMethods.erase(method);
	mMtxRespondedMethods.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Added responded method "<<method);
}

bool AppLinkInterface::findAwaitedResponseMethod( int method ) const
{
	return mAwaitedResponseMethods.find(method) != mAwaitedResponseMethods.end();
}

bool AppLinkInterface::findRespondedMethod( int method ) const
{
	return mRespondedMethods.find(method) != mRespondedMethods.end();
}

void AppLinkInterface::receiveRPCCommand( RPC2Communication::RPC2Command* rpcObject )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Receiving RPC Bus object "<< rpcObject->getMethod() <<" from HMI...");

	int method = rpcObject->getMethod();
	if( findAwaitedResponseMethod(method) )
	{
		removeAwaitedResponseMethod(method);
		addRespondedMethod(method);
	}

	AppMgrCore::getInstance().pushRPC2CommunicationMessage(rpcObject);
	
	LOG4CPLUS_INFO_EXT(mLogger, " Received RPC Bus object "<< rpcObject->getMethod() <<" from HMI");
}

void AppLinkInterface::sendMessageAwaitingExecution( RPC2Communication::RPC2Command* rpcObject, bool repeat )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Sending a message awaiting result "<< rpcObject->getMethod() <<" to HMI...");
	
	enqueueRPCCommandOutgoing( rpcObject );
	int method = rpcObject->getMethod();
	addAwaitedResponseMethod(method);

	while( !findRespondedMethod(method) )
	{
		if( repeat )
		{
			enqueueRPCCommandOutgoing( rpcObject );
		}
	}

	removeRespondedMethod(method);

	LOG4CPLUS_INFO_EXT(mLogger, " Sent a message awaiting result "<< rpcObject->getMethod() <<" to HMI...");
}

void AppLinkInterface::getButtonCapabilities()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Getting button capabilities...");
	RPC2Communication::RPC2Command* rpcObject; //= RPC2Communication::RPC2Marshaller:: new RPCBusObject( 1, RPCBusObject::REQUEST, "Buttons.getCapabilities" );
	sendMessageAwaitingExecution( rpcObject, true );

	LOG4CPLUS_INFO_EXT(mLogger, " Getting button capabilities sent to HMI");
}

void AppLinkInterface::getVoiceCapabilities()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Getting voice capabilities...");
	RPC2Communication::RPC2Command* rpcObject; //= new RPCBusObject( 1, RPCBusObject::REQUEST, "Voice.getCapabilities" );
	sendMessageAwaitingExecution( rpcObject, true );
	
	LOG4CPLUS_INFO_EXT(mLogger, " Getting voice capabilities sent to HMI");
}

void AppLinkInterface::getVRCapabilities()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Getting VR capabilities...");
	RPC2Communication::RPC2Command* rpcObject; //= new RPCBusObject( 1, RPCBusObject::REQUEST, "VR.getCapabilities" );
	sendMessageAwaitingExecution( rpcObject, true );
	
	LOG4CPLUS_INFO_EXT(mLogger, " Getting VR capabilities sent to HMI");
}

void AppLinkInterface::getAllCapabilities()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Getting all capabilities");
	
	auto op1 = std::async(std::launch::async, &AppLinkInterface::getButtonCapabilities, this);
	auto op2 = std::async(std::launch::async, &AppLinkInterface::getVoiceCapabilities, this);
	auto op3 = std::async(std::launch::async, &AppLinkInterface::getVRCapabilities, this);

	while( !op1.valid() || !op2.valid() || !op3.valid() )
	{
	}

	LOG4CPLUS_INFO_EXT(mLogger, " Got all capabilities");
}

/** Thread manipulation */

void AppLinkInterface::executeThreads()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");
	mThreadRPCBusObjectsIncoming.Start(false);
	mThreadRPCBusObjectsOutgoing.Start(false);
	
	LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
	
	while(!m_bTerminate)
	{
	}
	
	LOG4CPLUS_INFO_EXT(mLogger, " Threads are being stopped!");
}

void AppLinkInterface::terminateThreads()
{
	m_bTerminate = true;
}

/** Thread functions */

void* AppLinkInterface::handleQueueRPCBusObjectsIncoming( void* )
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
			
			receiveRPCCommand( msg );
		}
	}
}

void* AppLinkInterface::handleQueueRPCBusObjectsOutgoing( void* )
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
			
			sendRPCCommand( msg );
		}
	}
}

/** Callbacks - upon message receiving */

/**
 * \brief pure virtual method to process response.
 * \param method method name which has been called.
 * \param root JSON message.
 */
void AppLinkInterface::processResponse(std::string method, Json::Value& root)
{
	LOG4CPLUS_INFO_EXT(mLogger, " A response to a method "<<method<<" has arrived");
	if(method == "Buttons.getCapabilities")
	{
		LOG4CPLUS_INFO_EXT(mLogger, " Buttons.getCapabilities response has been received!");
	}
	else if(method == "Voice.getCapabilities")
	{
		LOG4CPLUS_INFO_EXT(mLogger, " Voice.getCapabilities response has been received!");
	}
	else if(method == "VR.getCapabilities")
	{
		LOG4CPLUS_INFO_EXT(mLogger, " VR.getCapabilities response has been received!");
	}
	else
	{
		LOG4CPLUS_ERROR_EXT(mLogger, " An unknown method "<< method <<" response has been received!");
		return;
	}

	RPC2Communication::RPC2Command* object = RPC2Communication::RPC2Marshaller::fromJSON(root);
	enqueueRPCCommandIncoming(object);
}

/**
 * \brief pure virtual method to process request.
 * \param root JSON message.
 */
void AppLinkInterface::processRequest(Json::Value& root)
{
	LOG4CPLUS_INFO_EXT(mLogger, " A request has arrived");
	RPC2Communication::RPC2Command* object = RPC2Communication::RPC2Marshaller::fromJSON(root);
	enqueueRPCCommandIncoming(object);
}

/**
 * \brief Process notification message.
 * \brief Notify subscribers about property change.
 * expected notification format example:
 * \code
 * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
 * \endcode
 * \param root JSON message.
 */
void AppLinkInterface::processNotification(Json::Value& root)
{
	LOG4CPLUS_INFO_EXT(mLogger, " A notification has arrived");
	RPC2Communication::RPC2Command* object = RPC2Communication::RPC2Marshaller::fromJSON(root);
	enqueueRPCCommandIncoming(object);
}

};
