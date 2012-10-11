#include "AppMgr/AppMgr.h"

AppMgr& AppMgr::getInstance( )
{
	static AppMgr appMgr;
	return appMgr;
}

AppMgr::AppMgr()
{
}

AppMgr::~AppMgr()
{
}
