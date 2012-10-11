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

namespace NsAppManager
{
	
class AppMgrCore
{
public:
	~AppMgrCore();

	static AppMgrCore& getInstance();

private:
	AppMgrCore();
};

}; // namespace NsAppManager

#endif /* APPMGR_H_ */
