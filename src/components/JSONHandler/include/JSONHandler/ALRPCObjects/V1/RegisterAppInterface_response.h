#ifndef NSAPPLINKRPC_REGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define NSAPPLINKRPC_REGISTERAPPINTERFACE_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "ButtonCapabilities.h"
#include "DisplayCapabilities.h"
#include "HmiZoneCapabilities.h"
#include "Language.h"
#include "Result.h"
#include "SpeechCapabilities.h"
#include "SyncMsgVersion.h"
#include "VrCapabilities.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  The response to registerAppInterface

  class RegisterAppInterface_response : public ALRPCResponse
  {
  public:
  
    RegisterAppInterface_response(const RegisterAppInterface_response& c);
    RegisterAppInterface_response(void);
    
    virtual ~RegisterAppInterface_response(void);
  
    RegisterAppInterface_response& operator =(const RegisterAppInterface_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;
    const SyncMsgVersion* get_syncMsgVersion(void) const;
    const std::string* get_autoActivateID(void) const;
    const Language* get_language(void) const;
    const DisplayCapabilities* get_displayCapabilities(void) const;
    const std::vector<ButtonCapabilities>* get_buttonCapabilities(void) const;
    const std::vector<HmiZoneCapabilities>* get_hmiZoneCapabilities(void) const;
    const std::vector<SpeechCapabilities>* get_speechCapabilities(void) const;
    const std::vector<VrCapabilities>* get_vrCapabilities(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    void reset_syncMsgVersion(void);
    bool set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_);
    void reset_autoActivateID(void);
    bool set_autoActivateID(const std::string& autoActivateID_);
    void reset_language(void);
    bool set_language(const Language& language_);
    void reset_displayCapabilities(void);
    bool set_displayCapabilities(const DisplayCapabilities& displayCapabilities_);
    void reset_buttonCapabilities(void);
    bool set_buttonCapabilities(const std::vector<ButtonCapabilities>& buttonCapabilities_);
    void reset_hmiZoneCapabilities(void);
    bool set_hmiZoneCapabilities(const std::vector<HmiZoneCapabilities>& hmiZoneCapabilities_);
    void reset_speechCapabilities(void);
    bool set_speechCapabilities(const std::vector<SpeechCapabilities>& speechCapabilities_);
    void reset_vrCapabilities(void);
    bool set_vrCapabilities(const std::vector<VrCapabilities>& vrCapabilities_);

  private:
  
    friend class RegisterAppInterface_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

///  See SyncMsgVersion
      SyncMsgVersion* syncMsgVersion;

///  Provides an id that the app may use on a subsequent registerAppInterface to allow for auto activation (for example after an ignition cycle).
      std::string* autoActivateID;	//!< (16)

///  The currently active language on Sync. See "Language" for options.
      Language* language;

///  See DisplayCapabilities
      DisplayCapabilities* displayCapabilities;

///  See ButtonCapabilities
      std::vector<ButtonCapabilities>* buttonCapabilities;	//!<   [%s..%s] 

///  See HmiZoneCapabilities
      std::vector<HmiZoneCapabilities>* hmiZoneCapabilities;	//!<   [%s..%s] 

///  See SpeechCapabilities
      std::vector<SpeechCapabilities>* speechCapabilities;	//!<   [%s..%s] 

///  See VrCapabilities
      std::vector<VrCapabilities>* vrCapabilities;	//!<   [%s..%s] 
  };

}

#endif
