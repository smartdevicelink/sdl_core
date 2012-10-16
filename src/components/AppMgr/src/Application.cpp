/*
 * Application.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/Application.h"

namespace NsAppManager
{
	
Application::Application( const std::string& name )
	:IApplication(name)
	,mNgnMediaScreenAppName("")
	,m_bUsesVehicleData(false)
	,m_bIsMediaApplication(false)
	,mAutoActivateID("")
{
}

Application::~Application( )
{
	// TODO Auto-generated destructor stub
}

void Application::setApplicationPriority( const AppPriority& priority )
{
	mPriority = priority;
}

};
