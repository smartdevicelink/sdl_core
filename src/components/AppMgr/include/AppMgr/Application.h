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
#include <string>
#include <vector>

class Language;
class SyncMsgVersion;

namespace NsAppManager
{
	
class Application : public IApplication
{
public:
	Application( const std::string& name );
	virtual ~Application( );
	virtual void setApplicationPriority( const AppPriority& priority );

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
};

}; // namespace NsAppManager

#endif /* APPLICATION_H_ */
