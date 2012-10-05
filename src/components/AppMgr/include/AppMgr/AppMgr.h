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

class AppMgr
{
public:
	~AppMgr();

	static AppMgr& getInstance();

private:
	AppMgr();
};


#endif /* APPMGR_H_ */
