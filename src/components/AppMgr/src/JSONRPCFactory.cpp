/*
 * JSONRPCFactory.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/JSONRPCFactory.h"

static JSONRPCFactory& JSONRPCFactory::getInstance( )
{
	static JSONRPCFactory factory;
	return factory;
}

JSONRPCFactory::JSONRPCFactory( )
{
	// TODO Auto-generated constructor stub

}

JSONRPCFactory::~JSONRPCFactory( )
{
	// TODO Auto-generated destructor stub
}

