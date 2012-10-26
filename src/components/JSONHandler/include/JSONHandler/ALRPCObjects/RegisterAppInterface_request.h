#ifndef REGISTERAPPINTERFACE_REQUEST_INCLUDE
#define REGISTERAPPINTERFACE_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Language.h"
#include "SyncMsgVersion.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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
  const Language& get_languageDesired(void) const;
  const std::string* get_autoActivateID(void) const;

  bool set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_);
  bool set_appName(const std::string& appName_);
  void reset_ngnMediaScreenAppName(void);
  bool set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_);
  void reset_vrSynonyms(void);
  bool set_vrSynonyms(const std::vector<std::string>& vrSynonyms_);
  void reset_usesVehicleData(void);
  bool set_usesVehicleData(bool usesVehicleData_);
  bool set_isMediaApplication(bool isMediaApplication_);
  bool set_languageDesired(const Language& languageDesired_);
  void reset_autoActivateID(void);
  bool set_autoActivateID(const std::string& autoActivateID_);

private:

  friend class RegisterAppInterface_requestMarshaller;

  SyncMsgVersion syncMsgVersion;
  std::string appName;	//!< (100)
  std::string* ngnMediaScreenAppName;	//!< (100)
  std::vector<std::string>* vrSynonyms;	//!<   [%s..%s] (40)
  bool* usesVehicleData;
  bool isMediaApplication;
  Language languageDesired;
  std::string* autoActivateID;	//!< (16)
};

#endif
