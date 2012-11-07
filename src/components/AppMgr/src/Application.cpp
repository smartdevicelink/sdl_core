/*
 * Application.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/Application.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
	
log4cplus::Logger Application::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Application"));

Application::Application(const std::string& name , unsigned char sessionId)
	:IApplication(name)
	,mNgnMediaScreenAppName("")
	,m_bUsesVehicleData(false)
	,m_bIsMediaApplication(false)
	,mAutoActivateID("")
    ,mSessionID(sessionId)
{
    LOG4CPLUS_INFO_EXT(mLogger, " Created an application "<< name <<" for the session id "<<sessionId);
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
    ,mSessionID(app.getSessionID())
{
	mVrSynonyms = app.getVrSynonyms();
}

Application::~Application( )
{
    LOG4CPLUS_INFO_EXT(mLogger, " Deleted an application "<< mName << " session id "<<mSessionID);
}

void Application::setApplicationPriority( const AppPriority& priority )
{
	mPriority = priority;
}

const AppLinkRPC::HMILevel::HMILevelInternal& Application::getApplicationHMIStatusLevel( ) const
{
	return mHMIStatusLevel;
}

void Application::setApplicationHMIStatusLevel( const AppLinkRPC::HMILevel::HMILevelInternal& hmiLevel )
{
	mHMIStatusLevel = hmiLevel;
}

const AppLinkRPC::AudioStreamingState& Application::getApplicationAudioStreamingState( ) const
{
	return mAudioStreamingState;
}

void Application::setApplicationAudioStreamingState( const AppLinkRPC::AudioStreamingState& streamingState )
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

void Application::setLanguageDesired(AppLinkRPC::Language value)
{
	mLanguageDesired = value;
}

void Application::setAutoActivateID(const std::string& value)
{
	mAutoActivateID = value;
}

void Application::setSyncMsgVersion(AppLinkRPC::SyncMsgVersion value)
{
    mSyncMsgVersion = value;
}

void Application::setAppID(const std::string &value)
{
    mAppID = value;
}

void Application::setHMIDisplayLanguageDesired(AppLinkRPC::Language value)
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

const AppLinkRPC::Language& Application::getLanguageDesired( ) const
{
	return mLanguageDesired;
}

const std::string& Application::getAutoActivateID( ) const
{
	return mAutoActivateID;
}

const AppLinkRPC::SyncMsgVersion& Application::getSyncMsgVersion( ) const
{
    return mSyncMsgVersion;
}

const std::string &Application::getAppID() const
{
    return mAppID;
}

const AppLinkRPC::Language &Application::getHMIDisplayLanguageDesired() const
{
    return mHMIDisplayLanguageDesired;
}

unsigned char Application::getSessionID() const
{
    return mSessionID;
}

}
