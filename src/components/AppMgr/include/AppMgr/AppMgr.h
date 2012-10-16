#ifndef APPMGR_H
#define APPMGR_H

#include "AppMgr/AppFactory.h"
#include "AppMgr/AppLinkInterface.h"
#include "AppMgr/AppMgrCore.h"
#include "AppMgr/RPCAppLinkFactory.h"
#include "AppMgr/RPCBusFactory.h"
#include "AppMgr/AppMgrRegistry.h"
#include "JSONHandler/IRPCMessagesObserver.h"
#include "mb_controller.hpp"

namespace NsAppManager
{

class AppMgr: public IRPCMessagesObserver, public CMessageBrokerController
{
public:
	AppMgr();
	static AppMgr& getInstance();
	virtual void onMessageReceivedCallback( MobileRPCMessage * message );
	
private:
	virtual ~AppMgr();

	AppLinkInterface& mAppLinkInterface;
	AppMgrRegistry& mAppMgrRegistry;
	AppMgrCore& mAppMgrCore;
	RPCAppLinkFactory& mRPCAppLinkFactory;
	RPCBusFactory& mRPCBusFactory;
	AppFactory& mAppFactory;
};

}; // namespace NsAppManager

#endif // APPMGR_H
