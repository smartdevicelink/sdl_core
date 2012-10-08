/*
 * RPCBusFactory.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef RPCBUSFACTORY_H_
#define RPCBUSFACTORY_H_

#include "AppMgr/RPCBusObject.h"

class RPCBusFactory
{
public:
	~RPCBusFactory( );

	static RPCBusFactory& getInstance();
	RPCBusObject* createNewRPCBusObject();

private:
	RPCBusFactory( );
};

#endif /* RPCBUSFACTORY_H_ */
