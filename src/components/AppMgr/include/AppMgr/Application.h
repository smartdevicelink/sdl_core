/*
 * Application.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "IApplication.h"
#include "JSONHandler/ALRPCObjects/Language.h"
#include "JSONHandler/ALRPCObjects/SyncMsgVersion.h"
#include "JSONHandler/ALRPCObjects/HMILevel.h"
#include "JSONHandler/ALRPCObjects/AudioStreamingState.h"
#include <string>
#include <vector>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

class Application : public IApplication
{
public:
    Application( const std::string& name, unsigned char sessionId );
	virtual ~Application( );
	virtual void setApplicationPriority( const AppPriority& priority );
	void setApplicationHMIStatusLevel( const AppLinkRPC::HMILevel::HMILevelInternal& hmiLevel );
	const AppLinkRPC::HMILevel::HMILevelInternal& getApplicationHMIStatusLevel( ) const;
	void setApplicationAudioStreamingState( const AppLinkRPC::AudioStreamingState& hmiLevel );
	const AppLinkRPC::AudioStreamingState& getApplicationAudioStreamingState( ) const;

	void setNgnMediaScreenAppName(const std::string& value);
	void setVrSynonyms(const std::vector<std::string>& value);
	void setUsesVehicleData(bool value);
	void setIsMediaApplication(bool value);
	void setLanguageDesired(AppLinkRPC::Language value);
	void setAutoActivateID(const std::string& value);
	void setSyncMsgVersion(AppLinkRPC::SyncMsgVersion value);
    void setAppID( const std::string& value );
    void setHMIDisplayLanguageDesired( AppLinkRPC::Language value );

	const std::string& getNgnMediaScreenAppName( ) const;
	const std::vector<std::string>& getVrSynonyms( ) const;
	bool getUsesVehicleData( ) const;
	bool getIsMediaApplication( ) const;
	const AppLinkRPC::Language& getLanguageDesired( ) const;
	const std::string& getAutoActivateID( ) const;
	const AppLinkRPC::SyncMsgVersion& getSyncMsgVersion( ) const;
    const std::string& getAppID( ) const;
    const AppLinkRPC::Language& getHMIDisplayLanguageDesired( ) const;
    unsigned char getSessionID() const;

private:
	Application(const Application& );
	
    const unsigned char mSessionID;
	std::string mNgnMediaScreenAppName;
	std::vector<std::string> mVrSynonyms;
	bool m_bUsesVehicleData;
	bool m_bIsMediaApplication;
	AppLinkRPC::Language mLanguageDesired;
	std::string mAutoActivateID;
	AppLinkRPC::SyncMsgVersion mSyncMsgVersion;
	AppLinkRPC::HMILevel::HMILevelInternal mHMIStatusLevel;
	AppLinkRPC::AudioStreamingState mAudioStreamingState;
    std::string mAppID;
    AppLinkRPC::Language mHMIDisplayLanguageDesired;

    static log4cplus::Logger mLogger;
};

} // namespace NsAppManager

#endif /* APPLICATION_H_ */
