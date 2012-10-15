/*
 * AppMgr.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPMGR_H_
#define APPMGR_H_

#include "IApplication.h"
#include "AppMgrRegistry.h"
#include "AppPolicy.h"
#include "RPCAppLinkObject.h"
#include "RPCBusObject.h"
#include "JSONHandler/IRPCMessagesObserver.h"
#include "JSONHandler/MobileRPCMessage.h"
#include "system.h"

#include <queue>

namespace NsAppManager
{
	
class AppMgrCore
{
public:
	~AppMgrCore();

	static AppMgrCore& getInstance();

private:
	AppMgrCore();

	std::queue< RPCAppLinkObject > mQueueRPCAppLinkObjectsIncoming;
	std::queue< RPCAppLinkObject > mQueueRPCAppLinkObjectsOutgoing;
	std::queue< RPCBusObject > mQueueRPCBusObjectsIncoming;
	std::queue< RPCBusObject > mQueueRPCBusObjectsOutgoing;
};

}; // namespace NsAppManager

#endif /* APPMGR_H_ */
