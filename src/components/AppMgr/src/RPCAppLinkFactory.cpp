/*
 * RPCAppLinkFactory.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/RPCAppLinkFactory.h"

namespace NsAppManager
{
	
RPCAppLinkFactory& RPCAppLinkFactory::getInstance( )
{
	static RPCAppLinkFactory factory;
	return factory;
}
/*
RPCAppLinkObject* RPCAppLinkFactory::createNewRPCAppLinkObject( )
{
	return 0;
}
*/
RPCAppLinkFactory::RPCAppLinkFactory( )
	:MobileRPCFactory()
{
	// TODO Auto-generated constructor stub

}

RPCAppLinkFactory::~RPCAppLinkFactory( )
{
	// TODO Auto-generated destructor stub
}

};
