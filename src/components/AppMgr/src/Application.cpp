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

const HMILevel::HMILevelEnum& Application::getApplicationHMIStatusLevel( ) const
{
	return mHMIStatusLevel;
}

void Application::setApplicationHMIStatusLevel( const HMILevel::HMILevelEnum& hmiLevel )
{
	mHMIStatusLevel = hmiLevel;
}

const AudioStreamingState& Application::getApplicationAudioStreamingState( ) const
{
	return mAudioStreamingState;
}

void Application::setApplicationAudioStreamingState( const AudioStreamingState& streamingState )
{
	mAudioStreamingState = streamingState;
}

void Application::setNgnMediaScreenAppName(const std::string& value)
{
	mNgnMediaScreenAppName = value;
}

void Application::setVrSynonyms(const std::vector<std::string>& value)
{
	mVrSynonyms = value;
}

void Application::setUsesVehicleData(bool value)
{
	m_bUsesVehicleData = value;
}

void Application::setIsMediaApplication(bool value)
{
	m_bIsMediaApplication = value;
}

void Application::setLanguageDesired(Language value)
{
	mLanguageDesired = value;
}

void Application::setAutoActivateID(const std::string& value)
{
	mAutoActivateID = value;
}

void Application::setSyncMsgVersion(SyncMsgVersion value)
{
	mSyncMsgVersion = value;
}

std::string Application::getNgnMediaScreenAppName( )
{
	return mNgnMediaScreenAppName;
}

std::vector<std::string> Application::getVrSynonyms( )
{
	return mVrSynonyms;
}

bool Application::getUsesVehicleData( )
{
	return m_bUsesVehicleData;
}

bool Application::getIsMediaApplication( )
{
	return m_bIsMediaApplication;
}

Language Application::getLanguageDesired( )
{
	return mLanguageDesired;
}

std::string Application::getAutoActivateID( )
{
	return mAutoActivateID;
}

SyncMsgVersion Application::getSyncMsgVersion( )
{
	return mSyncMsgVersion;
}

};
