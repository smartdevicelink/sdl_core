/*
 * RPCBusFactory.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/RPCBusFactory.h"

RPCBusFactory& RPCBusFactory::getInstance( )
{
	static RPCBusFactory factory;
	return factory;
}

RPCBusFactory::RPCBusFactory( )
{
	// TODO Auto-generated constructor stub

}

RPCBusFactory::~RPCBusFactory( )
{
	// TODO Auto-generated destructor stub
}

