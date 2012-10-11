#include "AppMgr/AppMgr.h"

namespace NsAppManager
{
	
AppMgr& AppMgr::getInstance( )
{
	static AppMgr appMgr;
	return appMgr;
}

AppMgr::AppMgr()
	:mAppLinkInterface(AppLinkInterface::getInstance())
	,mAppMgrRegistry(AppMgrRegistry::getInstance())
	,mAppMgrCore(AppMgrCore::getInstance())
	,mRPCAppLinkFactory(RPCAppLinkFactory::getInstance())
	,mRPCBusFactory(RPCBusFactory::getInstance())
	,mAppFactory(AppFactory::getInstance())
{
}

AppMgr::~AppMgr()
{
}

};
