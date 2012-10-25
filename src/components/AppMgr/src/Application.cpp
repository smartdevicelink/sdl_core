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

Application::Application( const Application& app )
	:IApplication(app.getName())
	,mNgnMediaScreenAppName(app.getNgnMediaScreenAppName())
	,m_bUsesVehicleData(app.getUsesVehicleData())
	,m_bIsMediaApplication(app.getIsMediaApplication())
	,mAutoActivateID(app.getAutoActivateID())
	,mLanguageDesired(app.getLanguageDesired())
	,mHMIStatusLevel(app.getApplicationHMIStatusLevel())
	,mSyncMsgVersion(app.getSyncMsgVersion())
{
	mVrSynonyms = app.getVrSynonyms();
}

Application::~Application( )
{
	// TODO Auto-generated destructor stub
}

void Application::setApplicationPriority( const AppPriority& priority )
{
	mPriority = priority;
}

const HMILevel::HMILevelInternal& Application::getApplicationHMIStatusLevel( ) const
{
	return mHMIStatusLevel;
}

void Application::setApplicationHMIStatusLevel( const HMILevel::HMILevelInternal& hmiLevel )
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

void Application::setAppID(const std::string &value)
{
    mAppID = value;
}

void Application::setHMIDisplayLanguageDesired(Language value)
{
    mHMIDisplayLanguageDesired = value;
}

const std::string& Application::getNgnMediaScreenAppName( ) const
{
	return mNgnMediaScreenAppName;
}

const std::vector<std::string>& Application::getVrSynonyms( ) const
{
	return mVrSynonyms;
}

bool Application::getUsesVehicleData( ) const
{
	return m_bUsesVehicleData;
}

bool Application::getIsMediaApplication( ) const
{
	return m_bIsMediaApplication;
}

const Language& Application::getLanguageDesired( ) const
{
	return mLanguageDesired;
}

const std::string& Application::getAutoActivateID( ) const
{
	return mAutoActivateID;
}

const SyncMsgVersion& Application::getSyncMsgVersion( ) const
{
    return mSyncMsgVersion;
}

const std::string &Application::getAppID() const
{
    return mAppID;
}

const Language &Application::getHMIDisplayLanguageDesired() const
{
    return mHMIDisplayLanguageDesired;
}

};
