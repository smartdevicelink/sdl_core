#ifndef REGISTERAPPINTERFACE_CLASS
#define REGISTERAPPINTERFACE_CLASS value

#include "JSONHandler/MobileRPCRequest.h"
#include "JSONHandler/SyncMsgVersion.h"
#include "JSONHandler/Language.h"
#include <vector>
#include <string>

class RegisterAppInterface : public MobileRPCRequest
{
public:
  RegisterAppInterface( unsigned int protocolVersion ):MobileRPCRequest( protocolVersion, "RegisterAppInterface" ){}
  ~RegisterAppInterface() {}

  void setSyncMsgVersion ( SyncMsgVersion syncMsgVersion ) { mSyncMsgVersion = syncMsgVersion; }
  void setAppName ( std::string appName ) { mAppName = appName; }
  void setNgnMediaScreenAppName( std::string ngnMediaScreenAppName ) { mNgnMediaScreenAppName = ngnMediaScreenAppName; }
  void setVrSynonyms ( std::vector<std::string> vrSynonyms ) { mVrSynonyms = vrSynonyms; }
  void setUsesVehicleData( bool usesVehicleData ) { mUsesVehicleData = usesVehicleData; }
  void setIsMediaApplication( bool isMediaApplication ) { mIsMediaApplication = isMediaApplication; }
  void setLanguageDesired( Language languageDesired ) { mLanguageDesired = languageDesired; }
  void setAutoActivateID( std::string autoActivateID ) { mAutoActivateID = autoActivateID; }

  SyncMsgVersion getSyncMsgVersion() const { return mSyncMsgVersion; }
  std::string getAppName() const { return mAppName; }
  std::string getNgnMediaScreenAppName() const { return mNgnMediaScreenAppName; }
  std::vector<std::string> getVrSynonyms() const { return mVrSynonyms; }
  bool getUsesVehicleData() const { return mUsesVehicleData; }
  bool getIsMediaApplication() const { return mIsMediaApplication; }
  Language getLanguageDesired() const { return mLanguageDesired; }
  std::string getAutoActivateID() const { return mAutoActivateID; }

  /* data */
private:
  SyncMsgVersion mSyncMsgVersion;
  std::string mAppName;
  std::string mNgnMediaScreenAppName;
  std::vector<std::string> mVrSynonyms;
  bool mUsesVehicleData;
  bool mIsMediaApplication;
  Language mLanguageDesired;
  std::string mAutoActivateID;

};

#endif