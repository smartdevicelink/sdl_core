#ifndef REGISTERAPPINTERFACE_REQUEST_INCLUDE
#define REGISTERAPPINTERFACE_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Language.h"
#include "SyncMsgVersion.h"
#include "JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


/**
     Establishes an interface with a mobile application.
     Before registerAppInterface no other commands will be accepted/executed.
*/

class RegisterAppInterface_request : public ALRPCRequest
{
public:

  RegisterAppInterface_request(const RegisterAppInterface_request& c);
  RegisterAppInterface_request(void);
  
  virtual ~RegisterAppInterface_request(void);

  RegisterAppInterface_request& operator =(const RegisterAppInterface_request&);

  bool checkIntegrity(void);

  const SyncMsgVersion& get_syncMsgVersion(void) const;
  const std::string& get_appName(void) const;
  const std::string* get_ngnMediaScreenAppName(void) const;
  const std::vector<std::string>* get_vrSynonyms(void) const;
  const bool* get_usesVehicleData(void) const;
  bool get_isMediaApplication(void) const;
  const Language* get_languageDesired(void) const;
  const Language* get_hmiDisplayLanguageDesired(void) const;
  const std::string* get_autoActivateID(void) const;
  const std::string* get_appID(void) const;

  bool set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_);
  bool set_appName(const std::string& appName_);
  void reset_ngnMediaScreenAppName(void);
  bool set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_);
  void reset_vrSynonyms(void);
  bool set_vrSynonyms(const std::vector<std::string>& vrSynonyms_);
  void reset_usesVehicleData(void);
  bool set_usesVehicleData(bool usesVehicleData_);
  bool set_isMediaApplication(bool isMediaApplication_);
  void reset_languageDesired(void);
  bool set_languageDesired(const Language& languageDesired_);
  void reset_hmiDisplayLanguageDesired(void);
  bool set_hmiDisplayLanguageDesired(const Language& hmiDisplayLanguageDesired_);
  void reset_autoActivateID(void);
  bool set_autoActivateID(const std::string& autoActivateID_);
  void reset_appID(void);
  bool set_appID(const std::string& appID_);

private:

  friend class RegisterAppInterface_requestMarshaller;

  SyncMsgVersion syncMsgVersion;
  std::string appName;	//!< (100)
  std::string* ngnMediaScreenAppName;	//!< (100)
  std::vector<std::string>* vrSynonyms;	//!<   [%s..%s] (40)
  bool* usesVehicleData;
  bool isMediaApplication;
  Language* languageDesired;
  Language* hmiDisplayLanguageDesired;
  std::string* autoActivateID;	//!< (16)
  std::string* appID;	//!< (100)
};

#endif
