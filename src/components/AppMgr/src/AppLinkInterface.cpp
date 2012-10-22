/*
 * AppLinkInterface.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgr.h"
#include "AppMgr/AppLinkInterface.h"
#include "CMessageBroker.hpp"
#include "CMessageBrokerRegistry.hpp"
#include "CSender.hpp"
#include <map>
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

void AppLinkInterface::sendRPCBusObject( const RPCBusObject* rpcObject )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Sending RPC Bus object "<< rpcObject->getMethodName() <<" to HMI...");
	Json::Value request, params;
	prepareMessage(request);
	request["method"] = rpcObject->getMethodName();

	if(!rpcObject->getParameters().empty())
	{
		for(RPCBusObject::Parameters::const_iterator it = rpcObject->getParameters().begin(); it != rpcObject->getParameters().end(); it++)
		{
			params[it->first] = it->second;
		}
		request["params"] = params;
	}
	
	sendJsonMessage(request);
	LOG4CPLUS_INFO_EXT(mLogger, " Sent RPC Bus object "<< rpcObject->getMethodName() <<" to HMI");
}

void AppLinkInterface::enqueueRPCBusObject( RPCBusObject * object )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Pushing RPC bus message...");
	mMtxRPCBusObjectsIncoming.Lock();
	
	mQueueRPCBusObjectsIncoming.push(object);
	
	mMtxRPCBusObjectsIncoming.Unlock();
	LOG4CPLUS_INFO_EXT(mLogger, " Pushed RPC bus message");
}

void AppLinkInterface::receiveRPCBusObject( const RPCBusObject* rpcObject )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Receiving RPC Bus object "<< rpcObject->getMethodName() <<" from HMI...");

	
	
	LOG4CPLUS_INFO_EXT(mLogger, " Received RPC Bus object "<< rpcObject->getMethodName() <<" from HMI");
}

void AppLinkInterface::sendMessage()
{
	
}

void AppLinkInterface::getButtonCapabilities()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Getting button capabilities...");
	RPCBusObject* rpcObject = new RPCBusObject( 1, RPCBusObject::REQUEST, "Buttons.getCapabilities" );

	LOG4CPLUS_INFO_EXT(mLogger, " Getting button capabilities sent to HMI");
}

void AppLinkInterface::getVoiceCapabilities()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Getting voice capabilities...");
	RPCBusObject* rpcObject = new RPCBusObject( 1, RPCBusObject::REQUEST, "Voice.getCapabilities" );
	
	LOG4CPLUS_INFO_EXT(mLogger, " Getting voice capabilities sent to HMI");
}

void AppLinkInterface::getVRCapabilities()
{
	LOG4CPLUS_INFO_EXT(mLogger, " Getting VR capabilities...");
	RPCBusObject* rpcObject = new RPCBusObject( 1, RPCBusObject::REQUEST, "VR.getCapabilities" );
	
	LOG4CPLUS_INFO_EXT(mLogger, " Getting VR capabilities sent to HMI");
}

void AppLinkInterface::getAllCapabilities()
{
	
}

RPCBusObject* AppLinkInterface::createObjectFromJson(const std::string& method, const Json::Value& root)
{
	RPCBusObject* rpcObject = new RPCBusObject();
	Json::Value value, params;
	value = root["correlationID"];
	if ( !value.isNull() )
	{
		rpcObject->setCorrelationID( value.asInt() );
	}

	if(method.length())
	{
		rpcObject->setMethodName(method);
	}
	else
	{
		value = root["name"];
		if ( !value.isNull() )
		{
			rpcObject->setMethodName( value.asString() );
		}
	}

	rpcObject->setProtocolVersion(1);

	params = root["params"];

	if(params.type() == Json::objectValue)
	{
		if(!params.empty())
		{
			for(Json::ValueIterator it = params.begin(); it != params.end(); it++)
			{
				rpcObject->setParameter(it.memberName(), (*it).asString());
			}
		}
	}

	return rpcObject;
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
			RPCBusObject* msg = mQueueRPCBusObjectsIncoming.front();
			mQueueRPCBusObjectsIncoming.pop();
			mMtxRPCBusObjectsIncoming.Unlock();
			if(!msg)
			{
				LOG4CPLUS_ERROR_EXT(mLogger, " Erroneous null-message has been received!");
				continue;
			}
			
			receiveRPCBusObject( msg );
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
			RPCBusObject* msg = mQueueRPCBusObjectsOutgoing.front();
			mQueueRPCBusObjectsOutgoing.pop();
			mMtxRPCBusObjectsOutgoing.Unlock();
			if(!msg)
			{
				LOG4CPLUS_ERROR_EXT(mLogger, " Erroneous null-message has been received!");
				continue;
			}
			
			sendRPCBusObject( msg );
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

	RPCBusObject* object = createObjectFromJson(method, root);
	enqueueRPCBusObject(object);
}

/**
 * \brief pure virtual method to process request.
 * \param root JSON message.
 */
void AppLinkInterface::processRequest(Json::Value& root)
{
	LOG4CPLUS_INFO_EXT(mLogger, " A request has arrived");
	RPCBusObject* object = createObjectFromJson("", root);
	enqueueRPCBusObject(object);
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
	RPCBusObject* object = createObjectFromJson("", root);
	enqueueRPCBusObject(object);
}

};
