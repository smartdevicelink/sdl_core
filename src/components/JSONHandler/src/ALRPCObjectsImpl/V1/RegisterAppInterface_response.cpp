#include "../include/JSONHandler/ALRPCObjects/V1/RegisterAppInterface_response.h"
#include "RegisterAppInterface_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "ButtonCapabilitiesMarshaller.h"
#include "DisplayCapabilitiesMarshaller.h"
#include "HmiZoneCapabilitiesMarshaller.h"
#include "LanguageMarshaller.h"
#include "ResultMarshaller.h"
#include "SpeechCapabilitiesMarshaller.h"
#include "SyncMsgVersionMarshaller.h"
#include "VrCapabilitiesMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
RegisterAppInterface_response& RegisterAppInterface_response::operator =(const RegisterAppInterface_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;
  syncMsgVersion= c.syncMsgVersion ? new SyncMsgVersion(c.syncMsgVersion[0]) : 0;
  autoActivateID= c.autoActivateID ? new std::string(c.autoActivateID[0]) : 0;
  language= c.language ? new Language(c.language[0]) : 0;
  displayCapabilities= c.displayCapabilities ? new DisplayCapabilities(c.displayCapabilities[0]) : 0;
  buttonCapabilities= c.buttonCapabilities ? new std::vector<ButtonCapabilities>(c.buttonCapabilities[0]) : 0;
  hmiZoneCapabilities= c.hmiZoneCapabilities ? new std::vector<HmiZoneCapabilities>(c.hmiZoneCapabilities[0]) : 0;
  speechCapabilities= c.speechCapabilities ? new std::vector<SpeechCapabilities>(c.speechCapabilities[0]) : 0;
  vrCapabilities= c.vrCapabilities ? new std::vector<VrCapabilities>(c.vrCapabilities[0]) : 0;

  return *this;}


RegisterAppInterface_response::~RegisterAppInterface_response(void)
{
  if(info)
    delete info;
  if(syncMsgVersion)
    delete syncMsgVersion;
  if(autoActivateID)
    delete autoActivateID;
  if(language)
    delete language;
  if(displayCapabilities)
    delete displayCapabilities;
  if(buttonCapabilities)
    delete buttonCapabilities;
  if(hmiZoneCapabilities)
    delete hmiZoneCapabilities;
  if(speechCapabilities)
    delete speechCapabilities;
  if(vrCapabilities)
    delete vrCapabilities;
}


RegisterAppInterface_response::RegisterAppInterface_response(const RegisterAppInterface_response& c)
{
  *this=c;
}


bool RegisterAppInterface_response::checkIntegrity(void)
{
  return RegisterAppInterface_responseMarshaller::checkIntegrity(*this);
}


RegisterAppInterface_response::RegisterAppInterface_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_REGISTERAPPINTERFACE_RESPONSE),
      info(0),
    syncMsgVersion(0),
    autoActivateID(0),
    language(0),
    displayCapabilities(0),
    buttonCapabilities(0),
    hmiZoneCapabilities(0),
    speechCapabilities(0),
    vrCapabilities(0)
{
}



bool RegisterAppInterface_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool RegisterAppInterface_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool RegisterAppInterface_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void RegisterAppInterface_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}

bool RegisterAppInterface_response::set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_)
{
  if(!SyncMsgVersionMarshaller::checkIntegrityConst(syncMsgVersion_))   return false;
  delete syncMsgVersion;
  syncMsgVersion=0;

  syncMsgVersion=new SyncMsgVersion(syncMsgVersion_);
  return true;
}

void RegisterAppInterface_response::reset_syncMsgVersion(void)
{
  if(syncMsgVersion)
    delete syncMsgVersion;
  syncMsgVersion=0;
}

bool RegisterAppInterface_response::set_autoActivateID(const std::string& autoActivateID_)
{
  if(autoActivateID_.length()>16)  return false;
  delete autoActivateID;
  autoActivateID=0;

  autoActivateID=new std::string(autoActivateID_);
  return true;
}

void RegisterAppInterface_response::reset_autoActivateID(void)
{
  if(autoActivateID)
    delete autoActivateID;
  autoActivateID=0;
}

bool RegisterAppInterface_response::set_language(const Language& language_)
{
  if(!LanguageMarshaller::checkIntegrityConst(language_))   return false;
  delete language;
  language=0;

  language=new Language(language_);
  return true;
}

void RegisterAppInterface_response::reset_language(void)
{
  if(language)
    delete language;
  language=0;
}

bool RegisterAppInterface_response::set_displayCapabilities(const DisplayCapabilities& displayCapabilities_)
{
  if(!DisplayCapabilitiesMarshaller::checkIntegrityConst(displayCapabilities_))   return false;
  delete displayCapabilities;
  displayCapabilities=0;

  displayCapabilities=new DisplayCapabilities(displayCapabilities_);
  return true;
}

void RegisterAppInterface_response::reset_displayCapabilities(void)
{
  if(displayCapabilities)
    delete displayCapabilities;
  displayCapabilities=0;
}

bool RegisterAppInterface_response::set_buttonCapabilities(const std::vector<ButtonCapabilities>& buttonCapabilities_)
{
  unsigned int i=buttonCapabilities_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!ButtonCapabilitiesMarshaller::checkIntegrityConst(buttonCapabilities_[i]))   return false;
  }
  delete buttonCapabilities;
  buttonCapabilities=0;

  buttonCapabilities=new std::vector<ButtonCapabilities>(buttonCapabilities_);
  return true;
}

void RegisterAppInterface_response::reset_buttonCapabilities(void)
{
  if(buttonCapabilities)
    delete buttonCapabilities;
  buttonCapabilities=0;
}

bool RegisterAppInterface_response::set_hmiZoneCapabilities(const std::vector<HmiZoneCapabilities>& hmiZoneCapabilities_)
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

void RegisterAppInterface_response::reset_hmiZoneCapabilities(void)
{
  if(hmiZoneCapabilities)
    delete hmiZoneCapabilities;
  hmiZoneCapabilities=0;
}

bool RegisterAppInterface_response::set_speechCapabilities(const std::vector<SpeechCapabilities>& speechCapabilities_)
{
  unsigned int i=speechCapabilities_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!SpeechCapabilitiesMarshaller::checkIntegrityConst(speechCapabilities_[i]))   return false;
  }
  delete speechCapabilities;
  speechCapabilities=0;

  speechCapabilities=new std::vector<SpeechCapabilities>(speechCapabilities_);
  return true;
}

void RegisterAppInterface_response::reset_speechCapabilities(void)
{
  if(speechCapabilities)
    delete speechCapabilities;
  speechCapabilities=0;
}

bool RegisterAppInterface_response::set_vrCapabilities(const std::vector<VrCapabilities>& vrCapabilities_)
{
  unsigned int i=vrCapabilities_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!VrCapabilitiesMarshaller::checkIntegrityConst(vrCapabilities_[i]))   return false;
  }
  delete vrCapabilities;
  vrCapabilities=0;

  vrCapabilities=new std::vector<VrCapabilities>(vrCapabilities_);
  return true;
}

void RegisterAppInterface_response::reset_vrCapabilities(void)
{
  if(vrCapabilities)
    delete vrCapabilities;
  vrCapabilities=0;
}




bool RegisterAppInterface_response::get_success(void) const
{
  return success;
}

const Result& RegisterAppInterface_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* RegisterAppInterface_response::get_info(void) const 
{
  return info;
}

const SyncMsgVersion* RegisterAppInterface_response::get_syncMsgVersion(void) const 
{
  return syncMsgVersion;
}

const std::string* RegisterAppInterface_response::get_autoActivateID(void) const 
{
  return autoActivateID;
}

const Language* RegisterAppInterface_response::get_language(void) const 
{
  return language;
}

const DisplayCapabilities* RegisterAppInterface_response::get_displayCapabilities(void) const 
{
  return displayCapabilities;
}

const std::vector<ButtonCapabilities>* RegisterAppInterface_response::get_buttonCapabilities(void) const 
{
  return buttonCapabilities;
}

const std::vector<HmiZoneCapabilities>* RegisterAppInterface_response::get_hmiZoneCapabilities(void) const 
{
  return hmiZoneCapabilities;
}

const std::vector<SpeechCapabilities>* RegisterAppInterface_response::get_speechCapabilities(void) const 
{
  return speechCapabilities;
}

const std::vector<VrCapabilities>* RegisterAppInterface_response::get_vrCapabilities(void) const 
{
  return vrCapabilities;
}

