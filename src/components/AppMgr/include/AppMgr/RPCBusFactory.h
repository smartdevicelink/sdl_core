/*
 * RPCBusFactory.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef RPCBUSFACTORY_H_
#define RPCBUSFACTORY_H_

class RPCBusFactory
{
public:
	~RPCBusFactory( );

	static RPCBusFactory& getInstance();

private:
	RPCBusFactory( );
};

#endif /* RPCBUSFACTORY_H_ */
