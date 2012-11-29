#ifndef NSAPPLINKRPC_REGISTERAPPINTERFACE_V2_RESPONSE_INCLUDE
#define NSAPPLINKRPC_REGISTERAPPINTERFACE_V2_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "ButtonCapabilities_v2.h"
#include "DisplayCapabilities_v2.h"
#include "HmiZoneCapabilities.h"
#include "Language_v2.h"
#include "PresetBankCapabilities.h"
#include "Result_v2.h"
#include "SoftButtonCapabilities.h"
#include "SpeechCapabilities_v2.h"
#include "SyncMsgVersion.h"
#include "VehicleType.h"
#include "VrCapabilities_v2.h"
#include "JSONHandler/ALRPCResponse.h"


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

///  The response to registerAppInterface

  class RegisterAppInterface_v2_response : public ALRPCResponse
  {
  public:
  
    RegisterAppInterface_v2_response(const RegisterAppInterface_v2_response& c);
    RegisterAppInterface_v2_response(void);
    
    virtual ~RegisterAppInterface_v2_response(void);
  
    RegisterAppInterface_v2_response& operator =(const RegisterAppInterface_v2_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result_v2& get_resultCode(void) const;
    const std::string* get_info(void) const;
    const SyncMsgVersion* get_syncMsgVersion(void) const;
    const Language_v2* get_language(void) const;
    const Language_v2* get_hmiDisplayLanguage(void) const;
    const DisplayCapabilities_v2* get_displayCapabilities(void) const;
    const std::vector<ButtonCapabilities_v2>* get_buttonCapabilities(void) const;
    const std::vector<SoftButtonCapabilities>* get_softButtonCapabilities(void) const;
    const PresetBankCapabilities* get_presetBankCapabilities(void) const;
    const std::vector<HmiZoneCapabilities>* get_hmiZoneCapabilities(void) const;
    const std::vector<SpeechCapabilities_v2>* get_speechCapabilities(void) const;
    const std::vector<VrCapabilities_v2>* get_vrCapabilities(void) const;
    const VehicleType* get_vehicleType(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result_v2& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    void reset_syncMsgVersion(void);
    bool set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_);
    void reset_language(void);
    bool set_language(const Language_v2& language_);
    void reset_hmiDisplayLanguage(void);
    bool set_hmiDisplayLanguage(const Language_v2& hmiDisplayLanguage_);
    void reset_displayCapabilities(void);
    bool set_displayCapabilities(const DisplayCapabilities_v2& displayCapabilities_);
    void reset_buttonCapabilities(void);
    bool set_buttonCapabilities(const std::vector<ButtonCapabilities_v2>& buttonCapabilities_);
    void reset_softButtonCapabilities(void);
    bool set_softButtonCapabilities(const std::vector<SoftButtonCapabilities>& softButtonCapabilities_);
    void reset_presetBankCapabilities(void);
    bool set_presetBankCapabilities(const PresetBankCapabilities& presetBankCapabilities_);
    void reset_hmiZoneCapabilities(void);
    bool set_hmiZoneCapabilities(const std::vector<HmiZoneCapabilities>& hmiZoneCapabilities_);
    void reset_speechCapabilities(void);
    bool set_speechCapabilities(const std::vector<SpeechCapabilities_v2>& speechCapabilities_);
    void reset_vrCapabilities(void);
    bool set_vrCapabilities(const std::vector<VrCapabilities_v2>& vrCapabilities_);
    void reset_vehicleType(void);
    bool set_vehicleType(const VehicleType& vehicleType_);

  private:
  
    friend class RegisterAppInterface_v2_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result_v2 resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

///  See SyncMsgVersion
      SyncMsgVersion* syncMsgVersion;

///  The currently active VR+TTS language on Sync. See "Language" for options.
      Language_v2* language;

///  The currently active display language on Sync. See "Language" for       options.
      Language_v2* hmiDisplayLanguage;

///  See DisplayCapabilities
      DisplayCapabilities_v2* displayCapabilities;

///  See ButtonCapabilities
      std::vector<ButtonCapabilities_v2>* buttonCapabilities;	//!<   [%s..%s] 

/**
     If returned, the platform supports on-screen SoftButtons.
     See SoftButtonCapabilities
*/
      std::vector<SoftButtonCapabilities>* softButtonCapabilities;	//!<   [%s..%s] 

/**
     If returned, the platform supports custom on-screen Presets.
     See PresetBankCapabilities
*/
      PresetBankCapabilities* presetBankCapabilities;

///  See HmiZoneCapabilities
      std::vector<HmiZoneCapabilities>* hmiZoneCapabilities;	//!<   [%s..%s] 

///  See SpeechCapabilities
      std::vector<SpeechCapabilities_v2>* speechCapabilities;	//!<   [%s..%s] 

///  See VrCapabilities
      std::vector<VrCapabilities_v2>* vrCapabilities;	//!<   [%s..%s] 

///  Specifies the vehicle's type. See VehicleType.
      VehicleType* vehicleType;
  };

}

#endif
