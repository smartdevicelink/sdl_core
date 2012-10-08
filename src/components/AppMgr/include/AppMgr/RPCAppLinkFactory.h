/*
 * RPCAppLinkFactory.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef RPCAPPLINKFACTORY_H_
#define RPCAPPLINKFACTORY_H_

class RPCAppLinkFactory
{
public:
	~RPCAppLinkFactory( );
	static RPCAppLinkFactory& getInstance();

private:
	RPCAppLinkFactory( );
};

#endif /* RPCAPPLINKFACTORY_H_ */
