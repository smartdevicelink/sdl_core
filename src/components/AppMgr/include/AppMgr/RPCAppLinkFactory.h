/*
 * RPCAppLinkFactory.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef RPCAPPLINKFACTORY_H_
#define RPCAPPLINKFACTORY_H_

#include "AppMgr/RPCAppLinkObject.h"
#include "JSONHandler/MobileRPCFactory.h"

namespace NsAppManager
{
	
class RPCAppLinkFactory: public MobileRPCFactory
{
public:
	virtual ~RPCAppLinkFactory( );
	static RPCAppLinkFactory& getInstance();

//	RPCAppLinkObject* createNewRPCAppLinkObject();

private:
	RPCAppLinkFactory( );
};

}; // namespace NsAppManager

#endif /* RPCAPPLINKFACTORY_H_ */
