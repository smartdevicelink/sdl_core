#ifndef NSAPPLINKRPC_REGISTERAPPINTERFACE_V2_REQUEST_INCLUDE
#define NSAPPLINKRPC_REGISTERAPPINTERFACE_V2_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "AppType.h"
#include "Language_v2.h"
#include "SyncMsgVersion.h"
#include "TTSChunk_v2.h"
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

  class RegisterAppInterface_v2_request : public ALRPCRequest
  {
  public:
  
    RegisterAppInterface_v2_request(const RegisterAppInterface_v2_request& c);
    RegisterAppInterface_v2_request(void);
    
    virtual ~RegisterAppInterface_v2_request(void);
  
    RegisterAppInterface_v2_request& operator =(const RegisterAppInterface_v2_request&);
  
    bool checkIntegrity(void);

    const SyncMsgVersion& get_syncMsgVersion(void) const;
    const std::string& get_appName(void) const;
    const std::vector<TTSChunk_v2>* get_ttsName(void) const;
    const std::string* get_ngnMediaScreenAppName(void) const;
    const std::vector<std::string>* get_vrSynonyms(void) const;
    bool get_isMediaApplication(void) const;
    const Language_v2* get_languageDesired(void) const;
    const Language_v2* get_hmiDisplayLanguageDesired(void) const;
    const std::vector<AppType>* get_appType(void) const;
    const std::string* get_appID(void) const;

    bool set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_);
    bool set_appName(const std::string& appName_);
    void reset_ttsName(void);
    bool set_ttsName(const std::vector<TTSChunk_v2>& ttsName_);
    void reset_ngnMediaScreenAppName(void);
    bool set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_);
    void reset_vrSynonyms(void);
    bool set_vrSynonyms(const std::vector<std::string>& vrSynonyms_);
    bool set_isMediaApplication(bool isMediaApplication_);
    void reset_languageDesired(void);
    bool set_languageDesired(const Language_v2& languageDesired_);
    void reset_hmiDisplayLanguageDesired(void);
    bool set_hmiDisplayLanguageDesired(const Language_v2& hmiDisplayLanguageDesired_);
    void reset_appType(void);
    bool set_appType(const std::vector<AppType>& appType_);
    void reset_appID(void);
    bool set_appID(const std::string& appID_);

  private:
  
    friend class RegisterAppInterface_v2_requestMarshaller;


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
     TTS string for VR recognition of the mobile application name, e.g. "Ford Drive Green".
     Meant to overcome any failing on speech engine in properly pronouncing / understanding app name.
     Needs to be unique over all applications.
     May not be empty.
     May not start with a new line character.
     May not interfere with any name or synonym of previously registered applications and the following list of words @TODO: Create list(global commands)
     Needs to be unique over all applications. Applications with the same name will be rejected.
     Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
*/
      std::vector<TTSChunk_v2>* ttsName;	//!<   [%s..%s] 

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
     Indicates if the application is a media or a non-media application.
     Only media applications will be able to stream audio to Sync that is audible outside of the BT media source.
*/
      bool isMediaApplication;

/**
     See Language
     Current app’s expected VR+TTS language
     If there is a mismatch with SYNC, the app will be able to change this registration with changeLanguageRegistration prior to app being brought into focus.
*/
      Language_v2* languageDesired;

/**
     See Language
     Current app’s expected display language
     If there is a mismatch with SYNC, the app will be able to change this registration with changeLanguageRegistration prior to app being brought into focus.
*/
      Language_v2* hmiDisplayLanguageDesired;

/**
     See AppType
     List of all applicable app types stating which classifications to be given to the app.
     e.g. for platforms like GEN2, this will determine which "corner(s)" the app can populate.
*/
      std::vector<AppType>* appType;	//!<   [%s..%s] 

///  ID used to validate app with policy table entries
      std::string* appID;	//!< (100)
  };

}

#endif
