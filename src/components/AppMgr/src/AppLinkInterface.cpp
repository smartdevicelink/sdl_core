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

namespace NsAppManager
{

std::string AppLinkInterface::mAddress = "";
uint16_t AppLinkInterface::mPort = 0;
std::string AppLinkInterface::mName = "";
bool AppLinkInterface::m_bInitialized = false;
	
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
	Json::Value request, params;
	prepareMessage(request);
	request["method"] = rpcObject->getMethodName();

	for(std::map<std::string, std::string>::const_iterator it = rpcObject->getParameters().begin(); it != rpcObject->getParameters().end(); it++)
	{
		params[it->first] = it->second;
	}
	request["params"] = params;

	sendJsonMessage(request);
}

void AppLinkInterface::sendMessage()
{
	
}

void AppLinkInterface::getButtonCapabilities()
{
	Json::Value request;
	prepareMessage(request);
	request["method"] = "Buttons.getCapabilities";
	sendJsonMessage(request);
}

void AppLinkInterface::getVoiceCapabilities()
{
	Json::Value request;
	prepareMessage(request);
	request["method"] = "Voice.getCapabilities";
	sendJsonMessage(request);
}

void AppLinkInterface::getVRCapabilities()
{
	Json::Value request;
	prepareMessage(request);
	request["method"] = "VR.getCapabilities";
	sendJsonMessage(request);
}

AppLinkInterface::AppLinkInterface( const std::string& address, uint16_t port, const std::string& name )
	:NsMessageBroker::CMessageBrokerController::CMessageBrokerController(address, port, name)
{
}

AppLinkInterface::~AppLinkInterface( )
{
}

/** Callbacks - upon message receiving */

/**
 * \brief pure virtual method to process response.
 * \param method method name which has been called.
 * \param root JSON message.
 */
void AppLinkInterface::processResponse(std::string method, Json::Value& root)
{
}

/**
 * \brief pure virtual method to process request.
 * \param root JSON message.
 */
void AppLinkInterface::processRequest(Json::Value& root)
{
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
}

};
