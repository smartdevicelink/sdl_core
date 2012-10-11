/*
 * AppFactory.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppFactory.h"

namespace NsAppManager
{
	
AppFactory& AppFactory::getInstance( )
{
	static AppFactory appFactory;
	return appFactory;
}

AppFactory::AppFactory( )
{
	// TODO Auto-generated constructor stub

}

AppFactory::~AppFactory( )
{
	// TODO Auto-generated destructor stub
}

};
