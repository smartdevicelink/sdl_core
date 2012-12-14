#ifndef NSAPPLINKRPCV2_REGISTERAPPINTERFACE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_REGISTERAPPINTERFACE_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "ButtonCapabilities.h"
#include "DisplayCapabilities.h"
#include "HmiZoneCapabilities.h"
#include "Language.h"
#include "PresetBankCapabilities.h"
#include "Result.h"
#include "SoftButtonCapabilities.h"
#include "SpeechCapabilities.h"
#include "SyncMsgVersion.h"
#include "VehicleType.h"
#include "VrCapabilities.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  The response to registerAppInterface

  class RegisterAppInterface_response : public NsAppLinkRPC::ALRPCMessage
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
    const Language* get_language(void) const;
    const Language* get_hmiDisplayLanguage(void) const;
    const DisplayCapabilities* get_displayCapabilities(void) const;
    const std::vector<ButtonCapabilities>* get_buttonCapabilities(void) const;
    const std::vector<SoftButtonCapabilities>* get_softButtonCapabilities(void) const;
    const PresetBankCapabilities* get_presetBankCapabilities(void) const;
    const std::vector<HmiZoneCapabilities>* get_hmiZoneCapabilities(void) const;
    const std::vector<SpeechCapabilities>* get_speechCapabilities(void) const;
    const std::vector<VrCapabilities>* get_vrCapabilities(void) const;
    const VehicleType* get_vehicleType(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    void reset_syncMsgVersion(void);
    bool set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_);
    void reset_language(void);
    bool set_language(const Language& language_);
    void reset_hmiDisplayLanguage(void);
    bool set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_);
    void reset_displayCapabilities(void);
    bool set_displayCapabilities(const DisplayCapabilities& displayCapabilities_);
    void reset_buttonCapabilities(void);
    bool set_buttonCapabilities(const std::vector<ButtonCapabilities>& buttonCapabilities_);
    void reset_softButtonCapabilities(void);
    bool set_softButtonCapabilities(const std::vector<SoftButtonCapabilities>& softButtonCapabilities_);
    void reset_presetBankCapabilities(void);
    bool set_presetBankCapabilities(const PresetBankCapabilities& presetBankCapabilities_);
    void reset_hmiZoneCapabilities(void);
    bool set_hmiZoneCapabilities(const std::vector<HmiZoneCapabilities>& hmiZoneCapabilities_);
    void reset_speechCapabilities(void);
    bool set_speechCapabilities(const std::vector<SpeechCapabilities>& speechCapabilities_);
    void reset_vrCapabilities(void);
    bool set_vrCapabilities(const std::vector<VrCapabilities>& vrCapabilities_);
    void reset_vehicleType(void);
    bool set_vehicleType(const VehicleType& vehicleType_);

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

///  The currently active VR+TTS language on Sync. See "Language" for options.
      Language* language;

///  The currently active display language on Sync. See "Language" for       options.
      Language* hmiDisplayLanguage;

///  See DisplayCapabilities
      DisplayCapabilities* displayCapabilities;

///  See ButtonCapabilities
      std::vector<ButtonCapabilities>* buttonCapabilities;	//!<   [%s..%s] 

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
      std::vector<SpeechCapabilities>* speechCapabilities;	//!<   [%s..%s] 

///  See VrCapabilities
      std::vector<VrCapabilities>* vrCapabilities;	//!<   [%s..%s] 

///  Specifies the vehicle's type. See VehicleType.
      VehicleType* vehicleType;
  };

}

#endif
