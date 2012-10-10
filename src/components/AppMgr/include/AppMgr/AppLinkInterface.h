/*
 * AppLinkInterface.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPLINKINTERFACE_H_
#define APPLINKINTERFACE_H_

#include "RPCBusObject.h"

class AppLinkInterface
{
public:
	static AppLinkInterface& getInstance( );
	~AppLinkInterface( );

	void sendRPCBusObject(const RPCBusObject* rpcObject);

private:
	AppLinkInterface( );

	void sendMessage();
};

#endif /* APPLINKINTERFACE_H_ */
