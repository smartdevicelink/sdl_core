/*
 * JSONAppLinkFactory.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/JSONAppLinkFactory.h"

JSONAppLinkFactory& JSONAppLinkFactory::getInstance( )
{
	static JSONAppLinkFactory factory;
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

