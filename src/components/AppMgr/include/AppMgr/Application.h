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

class Language;
class SyncMsgVersion;

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

	std::string getNgnMediaScreenAppName( );
	std::vector<std::string> getVrSynonyms( );
	bool getUsesVehicleData( );
	bool getIsMediaApplication( );
	Language getLanguageDesired( );
	std::string getAutoActivateID( );
	SyncMsgVersion getSyncMsgVersion( );

private:
	std::string mNgnMediaScreenAppName;
	std::vector<std::string> mVrSynonyms;
	bool m_bUsesVehicleData;
	bool m_bIsMediaApplication;
	Language mLanguageDesired;
	std::string mAutoActivateID;
	SyncMsgVersion mSyncMsgVersion;
	HMILevel::HMILevelEnum mHMIStatusLevel;
	AudioStreamingState mAudioStreamingState;
};

}; // namespace NsAppManager

#endif /* APPLICATION_H_ */
