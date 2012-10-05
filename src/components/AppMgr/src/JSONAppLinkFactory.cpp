/*
 * JSONAppLinkFactory.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/JSONAppLinkFactory.h"

static JSONAppLinkFactory& JSONAppLinkFactory::getInstance( )
{
	JSONAppLinkFactory factory;
	return factory;
}

JSONAppLinkFactory::JSONAppLinkFactory( )
{
	// TODO Auto-generated constructor stub

}

JSONAppLinkFactory::~JSONAppLinkFactory( )
{
	// TODO Auto-generated destructor stub
}

