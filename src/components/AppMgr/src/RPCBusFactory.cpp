/*
 * RPCBusFactory.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/RPCBusFactory.h"

namespace NsAppManager
{
	
RPCBusFactory& RPCBusFactory::getInstance( )
{
	static RPCBusFactory factory;
	return factory;
}

RPCBusObject* RPCBusFactory::createNewRPCBusObject( )
{
	return 0;
}

RPCBusFactory::RPCBusFactory( )
{
	// TODO Auto-generated constructor stub

}

RPCBusFactory::~RPCBusFactory( )
{
	// TODO Auto-generated destructor stub
}

};
