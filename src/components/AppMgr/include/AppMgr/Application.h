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
