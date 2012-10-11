/*
 * AppLinkInterface.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppLinkInterface.h"
#include "CMessageBroker.hpp"
#include "CMessageBrokerRegistry.hpp"
#include "CSender.hpp"

namespace NsAppManager
{
	
AppLinkInterface& AppLinkInterface::getInstance( )
{
	static AppLinkInterface instance;
	return instance;
}

void AppLinkInterface::sendRPCBusObject( const RPCBusObject* rpcObject )
{
	//NsMessageBroker::CMessageBrokerRegistry::getInstance()->
}

AppLinkInterface::AppLinkInterface( )
{
	// TODO Auto-generated constructor stub

}

AppLinkInterface::~AppLinkInterface( )
{
	// TODO Auto-generated destructor stub
}

};
