#ifndef NSAPPLINKRPC_REGISTERAPPINTERFACE_REQUEST_INCLUDE
#define NSAPPLINKRPC_REGISTERAPPINTERFACE_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Language.h"
#include "SyncMsgVersion.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

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


///  See SyncMsgVersion
      SyncMsgVersion syncMsgVersion;

/**
     The mobile application name, e.g. "Ford Drive Green".
     Needs to be unique over all applications.
     May not be empty.
     May not start with a new line character.
     May not interfere with any name or synonym of previously registered applications and the following list of words @TODO: Create list(global commands)
     Needs to be unique over all applications. Applications with the same name will be rejected.
     Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
*/
      std::string appName;	//!< (100)

/**
     Provides an abbreviated version of the app name (if needed), that will be displayed on the NGN media screen.
     If not provided, the appName is used instead (and will be truncated if too long)
     Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
*/
      std::string* ngnMediaScreenAppName;	//!< (100)

/**
     Defines an additional voice recognition command.
     May not interfere with any name or synonym of previously registered applications and the following list of words @TODO: Create list(global commands)
     Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
*/
      std::vector<std::string>* vrSynonyms;	//!<   [%s..%s] (40)

/**
     If not provided, the default is equal to False"
     Indicates if the mobile application wants to use vehicle data like GPS or speed.
*/
      bool* usesVehicleData;

/**
     Indicates if the application is a media or a non-media application.
     Only media applications will be able to stream audio to Sync that is audible outside of the BT media source.
*/
      bool isMediaApplication;

/**
     See Language
     If the language doesn't match the active language on Sync, it will be rejected.
     If the language is changed, while an app is registered, it will get disconnected.
*/
      Language languageDesired;

/**
     Used to support auto activation after an initial successful registerAppInterface (for example after an ignition cycle).
     The app should always provide the id that was provided by the most recent registerAppInterface response.
     If this is the first call to registerAppInterface, then do not provide this parameter at all.
     If not provided or not matching with the id of the last registerAppInterface response, the app will not be automatically put into foreground on startup.
*/
      std::string* autoActivateID;	//!< (16)
  };

}

#endif
