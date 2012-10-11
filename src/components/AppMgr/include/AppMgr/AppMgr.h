#ifndef APPMGR_H
#define APPMGR_H

#include "AppMgr/AppFactory.h"
#include "AppMgr/AppLinkInterface.h"
#include "AppMgr/AppMgrCore.h"
#include "AppMgr/RPCAppLinkFactory.h"
#include "AppMgr/RPCBusFactory.h"
#include "AppMgr/AppMgrRegistry.h"

class AppMgr
{
public:
	AppMgr();
	static AppMgr& getInstance();
private:
	~AppMgr();

	AppLinkInterface& mAppLinkInterface;
	AppMgrRegistry& mAppMgrRegistry;
	AppMgrCore& mAppMgrCore;
	RPCAppLinkFactory& mRPCAppLinkFactory;
	RPCBusFactory& mRPCBusFactory;
	AppFactory& mAppFactory;
};

#endif // APPMGR_H
