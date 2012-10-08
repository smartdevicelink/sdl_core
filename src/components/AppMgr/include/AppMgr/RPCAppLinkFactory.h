/*
 * RPCAppLinkFactory.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef RPCAPPLINKFACTORY_H_
#define RPCAPPLINKFACTORY_H_

#include "AppMgr/RPCAppLinkObject.h"

class RPCAppLinkFactory
{
public:
	~RPCAppLinkFactory( );
	static RPCAppLinkFactory& getInstance();

	RPCAppLinkObject* createNewRPCAppLinkObject();

private:
	RPCAppLinkFactory( );
};

#endif /* RPCAPPLINKFACTORY_H_ */
