/*
 * RPCAppLinkFactory.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/RPCAppLinkFactory.h"

RPCAppLinkFactory& RPCAppLinkFactory::getInstance( )
{
	static RPCAppLinkFactory factory;
	return factory;
}

RPCAppLinkFactory::RPCAppLinkFactory( )
{
	// TODO Auto-generated constructor stub

}

RPCAppLinkFactory::~RPCAppLinkFactory( )
{
	// TODO Auto-generated destructor stub
}

