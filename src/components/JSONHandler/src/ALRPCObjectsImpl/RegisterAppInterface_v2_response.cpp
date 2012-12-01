#include "../include/JSONHandler/ALRPCObjects/RegisterAppInterface_v2_response.h"
#include "RegisterAppInterface_v2_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ButtonCapabilities_v2Marshaller.h"
#include "DisplayCapabilities_v2Marshaller.h"
#include "HmiZoneCapabilitiesMarshaller.h"
#include "Language_v2Marshaller.h"
#include "PresetBankCapabilitiesMarshaller.h"
#include "Result_v2Marshaller.h"
#include "SoftButtonCapabilitiesMarshaller.h"
#include "SpeechCapabilities_v2Marshaller.h"
#include "SyncMsgVersionMarshaller.h"
#include "VehicleTypeMarshaller.h"
#include "VrCapabilities_v2Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
RegisterAppInterface_v2_response& RegisterAppInterface_v2_response::operator =(const RegisterAppInterface_v2_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;
  syncMsgVersion= c.syncMsgVersion ? new SyncMsgVersion(c.syncMsgVersion[0]) : 0;
  language= c.language ? new Language_v2(c.language[0]) : 0;
  hmiDisplayLanguage= c.hmiDisplayLanguage ? new Language_v2(c.hmiDisplayLanguage[0]) : 0;
  displayCapabilities= c.displayCapabilities ? new DisplayCapabilities_v2(c.displayCapabilities[0]) : 0;
  buttonCapabilities= c.buttonCapabilities ? new std::vector<ButtonCapabilities_v2>(c.buttonCapabilities[0]) : 0;
  softButtonCapabilities= c.softButtonCapabilities ? new std::vector<SoftButtonCapabilities>(c.softButtonCapabilities[0]) : 0;
  presetBankCapabilities= c.presetBankCapabilities ? new PresetBankCapabilities(c.presetBankCapabilities[0]) : 0;
  hmiZoneCapabilities= c.hmiZoneCapabilities ? new std::vector<HmiZoneCapabilities>(c.hmiZoneCapabilities[0]) : 0;
  speechCapabilities= c.speechCapabilities ? new std::vector<SpeechCapabilities_v2>(c.speechCapabilities[0]) : 0;
  vrCapabilities= c.vrCapabilities ? new std::vector<VrCapabilities_v2>(c.vrCapabilities[0]) : 0;
  vehicleType= c.vehicleType ? new VehicleType(c.vehicleType[0]) : 0;

  return *this;}


RegisterAppInterface_v2_response::~RegisterAppInterface_v2_response(void)
{
  if(info)
    delete info;
  if(syncMsgVersion)
    delete syncMsgVersion;
  if(language)
    delete language;
  if(hmiDisplayLanguage)
    delete hmiDisplayLanguage;
  if(displayCapabilities)
    delete displayCapabilities;
  if(buttonCapabilities)
    delete buttonCapabilities;
  if(softButtonCapabilities)
    delete softButtonCapabilities;
  if(presetBankCapabilities)
    delete presetBankCapabilities;
  if(hmiZoneCapabilities)
    delete hmiZoneCapabilities;
  if(speechCapabilities)
    delete speechCapabilities;
  if(vrCapabilities)
    delete vrCapabilities;
  if(vehicleType)
    delete vehicleType;
}


RegisterAppInterface_v2_response::RegisterAppInterface_v2_response(const RegisterAppInterface_v2_response& c)
{
  *this=c;
}


bool RegisterAppInterface_v2_response::checkIntegrity(void)
{
  return RegisterAppInterface_v2_responseMarshaller::checkIntegrity(*this);
}


RegisterAppInterface_v2_response::RegisterAppInterface_v2_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_REGISTERAPPINTERFACE_V2_RESPONSE),
      info(0),
    syncMsgVersion(0),
    language(0),
    hmiDisplayLanguage(0),
    displayCapabilities(0),
    buttonCapabilities(0),
    softButtonCapabilities(0),
    presetBankCapabilities(0),
    hmiZoneCapabilities(0),
    speechCapabilities(0),
    vrCapabilities(0),
    vehicleType(0)
{
}



bool RegisterAppInterface_v2_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool RegisterAppInterface_v2_response::set_resultCode(const Result_v2& resultCode_)
{
  if(!Result_v2Marshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool RegisterAppInterface_v2_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void RegisterAppInterface_v2_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}

bool RegisterAppInterface_v2_response::set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_)
{
  if(!SyncMsgVersionMarshaller::checkIntegrityConst(syncMsgVersion_))   return false;
  delete syncMsgVersion;
  syncMsgVersion=0;

  syncMsgVersion=new SyncMsgVersion(syncMsgVersion_);
  return true;
}

void RegisterAppInterface_v2_response::reset_syncMsgVersion(void)
{
  if(syncMsgVersion)
    delete syncMsgVersion;
  syncMsgVersion=0;
}

bool RegisterAppInterface_v2_response::set_language(const Language_v2& language_)
{
  if(!Language_v2Marshaller::checkIntegrityConst(language_))   return false;
  delete language;
  language=0;

  language=new Language_v2(language_);
  return true;
}

void RegisterAppInterface_v2_response::reset_language(void)
{
  if(language)
    delete language;
  language=0;
}

bool RegisterAppInterface_v2_response::set_hmiDisplayLanguage(const Language_v2& hmiDisplayLanguage_)
{
  if(!Language_v2Marshaller::checkIntegrityConst(hmiDisplayLanguage_))   return false;
  delete hmiDisplayLanguage;
  hmiDisplayLanguage=0;

  hmiDisplayLanguage=new Language_v2(hmiDisplayLanguage_);
  return true;
}

void RegisterAppInterface_v2_response::reset_hmiDisplayLanguage(void)
{
  if(hmiDisplayLanguage)
    delete hmiDisplayLanguage;
  hmiDisplayLanguage=0;
}

bool RegisterAppInterface_v2_response::set_displayCapabilities(const DisplayCapabilities_v2& displayCapabilities_)
{
  if(!DisplayCapabilities_v2Marshaller::checkIntegrityConst(displayCapabilities_))   return false;
  delete displayCapabilities;
  displayCapabilities=0;

  displayCapabilities=new DisplayCapabilities_v2(displayCapabilities_);
  return true;
}

void RegisterAppInterface_v2_response::reset_displayCapabilities(void)
{
  if(displayCapabilities)
    delete displayCapabilities;
  displayCapabilities=0;
}

bool RegisterAppInterface_v2_response::set_buttonCapabilities(const std::vector<ButtonCapabilities_v2>& buttonCapabilities_)
{
  unsigned int i=buttonCapabilities_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!ButtonCapabilities_v2Marshaller::checkIntegrityConst(buttonCapabilities_[i]))   return false;
  }
  delete buttonCapabilities;
  buttonCapabilities=0;

  buttonCapabilities=new std::vector<ButtonCapabilities_v2>(buttonCapabilities_);
  return true;
}

void RegisterAppInterface_v2_response::reset_buttonCapabilities(void)
{
  if(buttonCapabilities)
    delete buttonCapabilities;
  buttonCapabilities=0;
}

bool RegisterAppInterface_v2_response::set_softButtonCapabilities(const std::vector<SoftButtonCapabilities>& softButtonCapabilities_)
{
  unsigned int i=softButtonCapabilities_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!SoftButtonCapabilitiesMarshaller::checkIntegrityConst(softButtonCapabilities_[i]))   return false;
  }
  delete softButtonCapabilities;
  softButtonCapabilities=0;

  softButtonCapabilities=new std::vector<SoftButtonCapabilities>(softButtonCapabilities_);
  return true;
}

void RegisterAppInterface_v2_response::reset_softButtonCapabilities(void)
{
  if(softButtonCapabilities)
    delete softButtonCapabilities;
  softButtonCapabilities=0;
}

bool RegisterAppInterface_v2_response::set_presetBankCapabilities(const PresetBankCapabilities& presetBankCapabilities_)
{
  if(!PresetBankCapabilitiesMarshaller::checkIntegrityConst(presetBankCapabilities_))   return false;
  delete presetBankCapabilities;
  presetBankCapabilities=0;

  presetBankCapabilities=new PresetBankCapabilities(presetBankCapabilities_);
  return true;
}

void RegisterAppInterface_v2_response::reset_presetBankCapabilities(void)
{
  if(presetBankCapabilities)
    delete presetBankCapabilities;
  presetBankCapabilities=0;
}

bool RegisterAppInterface_v2_response::set_hmiZoneCapabilities(const std::vector<HmiZoneCapabilities>& hmiZoneCapabilities_)
{
  unsigned int i=hmiZoneCapabilities_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!HmiZoneCapabilitiesMarshaller::checkIntegrityConst(hmiZoneCapabilities_[i]))   return false;
  }
  delete hmiZoneCapabilities;
  hmiZoneCapabilities=0;

  hmiZoneCapabilities=new std::vector<HmiZoneCapabilities>(hmiZoneCapabilities_);
  return true;
}

void RegisterAppInterface_v2_response::reset_hmiZoneCapabilities(void)
{
  if(hmiZoneCapabilities)
    delete hmiZoneCapabilities;
  hmiZoneCapabilities=0;
}

bool RegisterAppInterface_v2_response::set_speechCapabilities(const std::vector<SpeechCapabilities_v2>& speechCapabilities_)
{
  unsigned int i=speechCapabilities_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!SpeechCapabilities_v2Marshaller::checkIntegrityConst(speechCapabilities_[i]))   return false;
  }
  delete speechCapabilities;
  speechCapabilities=0;

  speechCapabilities=new std::vector<SpeechCapabilities_v2>(speechCapabilities_);
  return true;
}

void RegisterAppInterface_v2_response::reset_speechCapabilities(void)
{
  if(speechCapabilities)
    delete speechCapabilities;
  speechCapabilities=0;
}

bool RegisterAppInterface_v2_response::set_vrCapabilities(const std::vector<VrCapabilities_v2>& vrCapabilities_)
{
  unsigned int i=vrCapabilities_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!VrCapabilities_v2Marshaller::checkIntegrityConst(vrCapabilities_[i]))   return false;
  }
  delete vrCapabilities;
  vrCapabilities=0;

  vrCapabilities=new std::vector<VrCapabilities_v2>(vrCapabilities_);
  return true;
}

void RegisterAppInterface_v2_response::reset_vrCapabilities(void)
{
  if(vrCapabilities)
    delete vrCapabilities;
  vrCapabilities=0;
}

bool RegisterAppInterface_v2_response::set_vehicleType(const VehicleType& vehicleType_)
{
  if(!VehicleTypeMarshaller::checkIntegrityConst(vehicleType_))   return false;
  delete vehicleType;
  vehicleType=0;

  vehicleType=new VehicleType(vehicleType_);
  return true;
}

void RegisterAppInterface_v2_response::reset_vehicleType(void)
{
  if(vehicleType)
    delete vehicleType;
  vehicleType=0;
}




bool RegisterAppInterface_v2_response::get_success(void) const
{
  return success;
}

const Result_v2& RegisterAppInterface_v2_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* RegisterAppInterface_v2_response::get_info(void) const 
{
  return info;
}

const SyncMsgVersion* RegisterAppInterface_v2_response::get_syncMsgVersion(void) const 
{
  return syncMsgVersion;
}

const Language_v2* RegisterAppInterface_v2_response::get_language(void) const 
{
  return language;
}

const Language_v2* RegisterAppInterface_v2_response::get_hmiDisplayLanguage(void) const 
{
  return hmiDisplayLanguage;
}

const DisplayCapabilities_v2* RegisterAppInterface_v2_response::get_displayCapabilities(void) const 
{
  return displayCapabilities;
}

const std::vector<ButtonCapabilities_v2>* RegisterAppInterface_v2_response::get_buttonCapabilities(void) const 
{
  return buttonCapabilities;
}

const std::vector<SoftButtonCapabilities>* RegisterAppInterface_v2_response::get_softButtonCapabilities(void) const 
{
  return softButtonCapabilities;
}

const PresetBankCapabilities* RegisterAppInterface_v2_response::get_presetBankCapabilities(void) const 
{
  return presetBankCapabilities;
}

const std::vector<HmiZoneCapabilities>* RegisterAppInterface_v2_response::get_hmiZoneCapabilities(void) const 
{
  return hmiZoneCapabilities;
}

const std::vector<SpeechCapabilities_v2>* RegisterAppInterface_v2_response::get_speechCapabilities(void) const 
{
  return speechCapabilities;
}

const std::vector<VrCapabilities_v2>* RegisterAppInterface_v2_response::get_vrCapabilities(void) const 
{
  return vrCapabilities;
}

const VehicleType* RegisterAppInterface_v2_response::get_vehicleType(void) const 
{
  return vehicleType;
}

