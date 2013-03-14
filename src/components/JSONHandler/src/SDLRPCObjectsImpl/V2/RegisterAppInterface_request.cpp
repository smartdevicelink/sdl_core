#include "../include/JSONHandler/SDLRPCObjects/V2/RegisterAppInterface_request.h"
#include "RegisterAppInterface_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "AppTypeMarshaller.h"
#include "LanguageMarshaller.h"
#include "SyncMsgVersionMarshaller.h"
#include "TTSChunkMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;
RegisterAppInterface_request& RegisterAppInterface_request::operator =(const RegisterAppInterface_request& c)
{
  syncMsgVersion= c.syncMsgVersion;
  appName= c.appName;
  ttsName= c.ttsName ? new std::vector<TTSChunk>(c.ttsName[0]) : 0;
  ngnMediaScreenAppName= c.ngnMediaScreenAppName ? new std::string(c.ngnMediaScreenAppName[0]) : 0;
  vrSynonyms= c.vrSynonyms ? new std::vector<std::string>(c.vrSynonyms[0]) : 0;
  isMediaApplication= c.isMediaApplication;
  languageDesired= c.languageDesired;
  hmiDisplayLanguageDesired= c.hmiDisplayLanguageDesired;
  appType= c.appType ? new std::vector<AppType>(c.appType[0]) : 0;
  appID= c.appID;

  return *this;
}


RegisterAppInterface_request::~RegisterAppInterface_request(void)
{
  if(ttsName)
    delete ttsName;
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  if(vrSynonyms)
    delete vrSynonyms;
  if(appType)
    delete appType;
}


RegisterAppInterface_request::RegisterAppInterface_request(const RegisterAppInterface_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool RegisterAppInterface_request::checkIntegrity(void)
{
  return RegisterAppInterface_requestMarshaller::checkIntegrity(*this);
}


RegisterAppInterface_request::RegisterAppInterface_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      ttsName(0),
    ngnMediaScreenAppName(0),
    vrSynonyms(0),
    appType(0)
{
}



bool RegisterAppInterface_request::set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_)
{
  if(!SyncMsgVersionMarshaller::checkIntegrityConst(syncMsgVersion_))   return false;
  syncMsgVersion=syncMsgVersion_;
  return true;
}

bool RegisterAppInterface_request::set_appName(const std::string& appName_)
{
  if(appName_.length()>100)  return false;
  appName=appName_;
  return true;
}

bool RegisterAppInterface_request::set_ttsName(const std::vector<TTSChunk>& ttsName_)
{
  unsigned int i=ttsName_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunkMarshaller::checkIntegrityConst(ttsName_[i]))   return false;
  }
  delete ttsName;
  ttsName=0;

  ttsName=new std::vector<TTSChunk>(ttsName_);
  return true;
}

void RegisterAppInterface_request::reset_ttsName(void)
{
  if(ttsName)
    delete ttsName;
  ttsName=0;
}

bool RegisterAppInterface_request::set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_)
{
  if(ngnMediaScreenAppName_.length()>100)  return false;
  delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;

  ngnMediaScreenAppName=new std::string(ngnMediaScreenAppName_);
  return true;
}

void RegisterAppInterface_request::reset_ngnMediaScreenAppName(void)
{
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;
}

bool RegisterAppInterface_request::set_vrSynonyms(const std::vector<std::string>& vrSynonyms_)
{
  unsigned int i=vrSynonyms_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(vrSynonyms_[i].length()>40)  return false;
  }
  delete vrSynonyms;
  vrSynonyms=0;

  vrSynonyms=new std::vector<std::string>(vrSynonyms_);
  return true;
}

void RegisterAppInterface_request::reset_vrSynonyms(void)
{
  if(vrSynonyms)
    delete vrSynonyms;
  vrSynonyms=0;
}

bool RegisterAppInterface_request::set_isMediaApplication(bool isMediaApplication_)
{
  isMediaApplication=isMediaApplication_;
  return true;
}

bool RegisterAppInterface_request::set_languageDesired(const Language& languageDesired_)
{
  if(!LanguageMarshaller::checkIntegrityConst(languageDesired_))   return false;
  languageDesired=languageDesired_;
  return true;
}

bool RegisterAppInterface_request::set_hmiDisplayLanguageDesired(const Language& hmiDisplayLanguageDesired_)
{
  if(!LanguageMarshaller::checkIntegrityConst(hmiDisplayLanguageDesired_))   return false;
  hmiDisplayLanguageDesired=hmiDisplayLanguageDesired_;
  return true;
}

bool RegisterAppInterface_request::set_appType(const std::vector<AppType>& appType_)
{
  unsigned int i=appType_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!AppTypeMarshaller::checkIntegrityConst(appType_[i]))   return false;
  }
  delete appType;
  appType=0;

  appType=new std::vector<AppType>(appType_);
  return true;
}

void RegisterAppInterface_request::reset_appType(void)
{
  if(appType)
    delete appType;
  appType=0;
}

bool RegisterAppInterface_request::set_appID(const std::string& appID_)
{
  if(appID_.length()>100)  return false;
  appID=appID_;
  return true;
}




const SyncMsgVersion& RegisterAppInterface_request::get_syncMsgVersion(void) const 
{
  return syncMsgVersion;
}

const std::string& RegisterAppInterface_request::get_appName(void) const 
{
  return appName;
}

const std::vector<TTSChunk>* RegisterAppInterface_request::get_ttsName(void) const 
{
  return ttsName;
}

const std::string* RegisterAppInterface_request::get_ngnMediaScreenAppName(void) const 
{
  return ngnMediaScreenAppName;
}

const std::vector<std::string>* RegisterAppInterface_request::get_vrSynonyms(void) const 
{
  return vrSynonyms;
}

bool RegisterAppInterface_request::get_isMediaApplication(void) const
{
  return isMediaApplication;
}

const Language& RegisterAppInterface_request::get_languageDesired(void) const 
{
  return languageDesired;
}

const Language& RegisterAppInterface_request::get_hmiDisplayLanguageDesired(void) const 
{
  return hmiDisplayLanguageDesired;
}

const std::vector<AppType>* RegisterAppInterface_request::get_appType(void) const 
{
  return appType;
}

const std::string& RegisterAppInterface_request::get_appID(void) const 
{
  return appID;
}

