/*
 * Application.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "IApplication.h"
#include "JSONHandler/Language.h"
#include "JSONHandler/SyncMsgVersion.h"
#include "JSONHandler/HMILevel.h"
#include <string>
#include <vector>

namespace NsAppManager
{

typedef enum
{
	SS_AUDIBLE,
	SS_NOT_AUDIBLE
} AudioStreamingState;

class Application : public IApplication
{
public:
	Application( const std::string& name );
	virtual ~Application( );
	virtual void setApplicationPriority( const AppPriority& priority );
	void setApplicationHMIStatusLevel( const HMILevel::HMILevelEnum& hmiLevel );
	const HMILevel::HMILevelEnum& getApplicationHMIStatusLevel( ) const;
	void setApplicationAudioStreamingState( const AudioStreamingState& hmiLevel );
	const AudioStreamingState& getApplicationAudioStreamingState( ) const;

	void setNgnMediaScreenAppName(const std::string& value);
	void setVrSynonyms(const std::vector<std::string>& value);
	void setUsesVehicleData(bool value);
	void setIsMediaApplication(bool value);
	void setLanguageDesired(Language value);
	void setAutoActivateID(const std::string& value);
	void setSyncMsgVersion(SyncMsgVersion value);
    void setAppID( const std::string& value );
    void setHMIDisplayLanguageDesired( Language value );

	const std::string& getNgnMediaScreenAppName( ) const;
	const std::vector<std::string>& getVrSynonyms( ) const;
	bool getUsesVehicleData( ) const;
	bool getIsMediaApplication( ) const;
	const Language& getLanguageDesired( ) const;
	const std::string& getAutoActivateID( ) const;
	const SyncMsgVersion& getSyncMsgVersion( ) const;
    const std::string& getAppID( ) const;
    const Language& getHMIDisplayLanguageDesired( ) const;

private:
	Application(const Application& );
	
	std::string mNgnMediaScreenAppName;
	std::vector<std::string> mVrSynonyms;
	bool m_bUsesVehicleData;
	bool m_bIsMediaApplication;
	Language mLanguageDesired;
	std::string mAutoActivateID;
	SyncMsgVersion mSyncMsgVersion;
	HMILevel::HMILevelEnum mHMIStatusLevel;
	AudioStreamingState mAudioStreamingState;
    std::string mAppID;
    Language mHMIDisplayLanguageDesired;
};

}; // namespace NsAppManager

#endif /* APPLICATION_H_ */
