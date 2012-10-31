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
    Application( const std::string& name, unsigned char sessionId );
	virtual ~Application( );
	virtual void setApplicationPriority( const AppPriority& priority );
	void setApplicationHMIStatusLevel( const HMILevel::HMILevelInternal& hmiLevel );
	const HMILevel::HMILevelInternal& getApplicationHMIStatusLevel( ) const;
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
    unsigned char getSessionID() const;

private:
	Application(const Application& );
	
    const unsigned char mSessionID;
	std::string mNgnMediaScreenAppName;
	std::vector<std::string> mVrSynonyms;
	bool m_bUsesVehicleData;
	bool m_bIsMediaApplication;
	Language mLanguageDesired;
	std::string mAutoActivateID;
	SyncMsgVersion mSyncMsgVersion;
	HMILevel::HMILevelInternal mHMIStatusLevel;
	AudioStreamingState mAudioStreamingState;
    std::string mAppID;
    Language mHMIDisplayLanguageDesired;
};

}; // namespace NsAppManager

#endif /* APPLICATION_H_ */
