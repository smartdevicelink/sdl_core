#include "../include/JSONHandler/ALRPCObjects/RegisterAppInterface_v2_request.h"
#include "RegisterAppInterface_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "AppTypeMarshaller.h"
#include "Language_v2Marshaller.h"
#include "SyncMsgVersionMarshaller.h"
#include "TTSChunk_v2Marshaller.h"

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
RegisterAppInterface_v2_request& RegisterAppInterface_v2_request::operator =(const RegisterAppInterface_v2_request& c)
{
  syncMsgVersion= c.syncMsgVersion;
  appName= c.appName;
  ttsName= c.ttsName ? new std::vector<TTSChunk_v2>(c.ttsName[0]) : 0;
  ngnMediaScreenAppName= c.ngnMediaScreenAppName ? new std::string(c.ngnMediaScreenAppName[0]) : 0;
  vrSynonyms= c.vrSynonyms ? new std::vector<std::string>(c.vrSynonyms[0]) : 0;
  isMediaApplication= c.isMediaApplication;
  languageDesired= c.languageDesired ? new Language_v2(c.languageDesired[0]) : 0;
  hmiDisplayLanguageDesired= c.hmiDisplayLanguageDesired ? new Language_v2(c.hmiDisplayLanguageDesired[0]) : 0;
  appType= c.appType ? new std::vector<AppType>(c.appType[0]) : 0;
  appID= c.appID ? new std::string(c.appID[0]) : 0;

  return *this;}


RegisterAppInterface_v2_request::~RegisterAppInterface_v2_request(void)
{
  if(ttsName)
    delete ttsName;
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  if(vrSynonyms)
    delete vrSynonyms;
  if(languageDesired)
    delete languageDesired;
  if(hmiDisplayLanguageDesired)
    delete hmiDisplayLanguageDesired;
  if(appType)
    delete appType;
  if(appID)
    delete appID;
}


RegisterAppInterface_v2_request::RegisterAppInterface_v2_request(const RegisterAppInterface_v2_request& c)
{
  *this=c;
}


bool RegisterAppInterface_v2_request::checkIntegrity(void)
{
  return RegisterAppInterface_v2_requestMarshaller::checkIntegrity(*this);
}


RegisterAppInterface_v2_request::RegisterAppInterface_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_REGISTERAPPINTERFACE_V2_REQUEST),
      ttsName(0),
    ngnMediaScreenAppName(0),
    vrSynonyms(0),
    languageDesired(0),
    hmiDisplayLanguageDesired(0),
    appType(0),
    appID(0)
{
}



bool RegisterAppInterface_v2_request::set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_)
{
  if(!SyncMsgVersionMarshaller::checkIntegrityConst(syncMsgVersion_))   return false;
  syncMsgVersion=syncMsgVersion_;
  return true;
}

bool RegisterAppInterface_v2_request::set_appName(const std::string& appName_)
{
  if(appName_.length()>100)  return false;
  appName=appName_;
  return true;
}

bool RegisterAppInterface_v2_request::set_ttsName(const std::vector<TTSChunk_v2>& ttsName_)
{
  unsigned int i=ttsName_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(ttsName_[i]))   return false;
  }
  delete ttsName;
  ttsName=0;

  ttsName=new std::vector<TTSChunk_v2>(ttsName_);
  return true;
}

void RegisterAppInterface_v2_request::reset_ttsName(void)
{
  if(ttsName)
    delete ttsName;
  ttsName=0;
}

bool RegisterAppInterface_v2_request::set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_)
{
  if(ngnMediaScreenAppName_.length()>100)  return false;
  delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;

  ngnMediaScreenAppName=new std::string(ngnMediaScreenAppName_);
  return true;
}

void RegisterAppInterface_v2_request::reset_ngnMediaScreenAppName(void)
{
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;
}

bool RegisterAppInterface_v2_request::set_vrSynonyms(const std::vector<std::string>& vrSynonyms_)
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

void RegisterAppInterface_v2_request::reset_vrSynonyms(void)
{
  if(vrSynonyms)
    delete vrSynonyms;
  vrSynonyms=0;
}

bool RegisterAppInterface_v2_request::set_isMediaApplication(bool isMediaApplication_)
{
  isMediaApplication=isMediaApplication_;
  return true;
}

bool RegisterAppInterface_v2_request::set_languageDesired(const Language_v2& languageDesired_)
{
  if(!Language_v2Marshaller::checkIntegrityConst(languageDesired_))   return false;
  delete languageDesired;
  languageDesired=0;

  languageDesired=new Language_v2(languageDesired_);
  return true;
}

void RegisterAppInterface_v2_request::reset_languageDesired(void)
{
  if(languageDesired)
    delete languageDesired;
  languageDesired=0;
}

bool RegisterAppInterface_v2_request::set_hmiDisplayLanguageDesired(const Language_v2& hmiDisplayLanguageDesired_)
{
  if(!Language_v2Marshaller::checkIntegrityConst(hmiDisplayLanguageDesired_))   return false;
  delete hmiDisplayLanguageDesired;
  hmiDisplayLanguageDesired=0;

  hmiDisplayLanguageDesired=new Language_v2(hmiDisplayLanguageDesired_);
  return true;
}

void RegisterAppInterface_v2_request::reset_hmiDisplayLanguageDesired(void)
{
  if(hmiDisplayLanguageDesired)
    delete hmiDisplayLanguageDesired;
  hmiDisplayLanguageDesired=0;
}

bool RegisterAppInterface_v2_request::set_appType(const std::vector<AppType>& appType_)
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

void RegisterAppInterface_v2_request::reset_appType(void)
{
  if(appType)
    delete appType;
  appType=0;
}

bool RegisterAppInterface_v2_request::set_appID(const std::string& appID_)
{
  if(appID_.length()>100)  return false;
  delete appID;
  appID=0;

  appID=new std::string(appID_);
  return true;
}

void RegisterAppInterface_v2_request::reset_appID(void)
{
  if(appID)
    delete appID;
  appID=0;
}




const SyncMsgVersion& RegisterAppInterface_v2_request::get_syncMsgVersion(void) const 
{
  return syncMsgVersion;
}

const std::string& RegisterAppInterface_v2_request::get_appName(void) const 
{
  return appName;
}

const std::vector<TTSChunk_v2>* RegisterAppInterface_v2_request::get_ttsName(void) const 
{
  return ttsName;
}

const std::string* RegisterAppInterface_v2_request::get_ngnMediaScreenAppName(void) const 
{
  return ngnMediaScreenAppName;
}

const std::vector<std::string>* RegisterAppInterface_v2_request::get_vrSynonyms(void) const 
{
  return vrSynonyms;
}

bool RegisterAppInterface_v2_request::get_isMediaApplication(void) const
{
  return isMediaApplication;
}

const Language_v2* RegisterAppInterface_v2_request::get_languageDesired(void) const 
{
  return languageDesired;
}

const Language_v2* RegisterAppInterface_v2_request::get_hmiDisplayLanguageDesired(void) const 
{
  return hmiDisplayLanguageDesired;
}

const std::vector<AppType>* RegisterAppInterface_v2_request::get_appType(void) const 
{
  return appType;
}

const std::string* RegisterAppInterface_v2_request::get_appID(void) const 
{
  return appID;
}

