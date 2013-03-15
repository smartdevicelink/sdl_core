//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/HMIApplication.h"
#include "HMIApplicationMarshaller.h"
#include "AppTypeMarshaller.h"
#include "LanguageMarshaller.h"
#include "TTSChunkMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

HMIApplication& HMIApplication::operator =(const HMIApplication& c)
{
  appId=c.appId;
  appName=c.appName;
  appType=c.appType ? new std::vector<AppType>(c.appType[0]) : 0;
  deviceName=c.deviceName;
  hmiDisplayLanguageDesired=c.hmiDisplayLanguageDesired;
  icon=c.icon ? new std::string(c.icon[0]) : 0;
  isMediaApplication=c.isMediaApplication;
  languageDesired=c.languageDesired;
  ngnMediaScreenAppName=c.ngnMediaScreenAppName ? new std::string(c.ngnMediaScreenAppName[0]) : 0;
  ttsName=c.ttsName ? new std::vector<TTSChunk>(c.ttsName[0]) : 0;
  vrSynonyms=c.vrSynonyms ? new std::vector<std::string>(c.vrSynonyms[0]) : 0;

  return *this;
}


HMIApplication::~HMIApplication(void)
{
  if(appType)
    delete appType;
  if(icon)
    delete icon;
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  if(ttsName)
    delete ttsName;
  if(vrSynonyms)
    delete vrSynonyms;
}


HMIApplication::HMIApplication(const HMIApplication& c)
{
  *this=c;
}


bool HMIApplication::checkIntegrity(void)
{
  return HMIApplicationMarshaller::checkIntegrity(*this);
}


HMIApplication::HMIApplication(void) :
    appType(0),
    icon(0),
    ngnMediaScreenAppName(0),
    ttsName(0),
    vrSynonyms(0)
{
}



bool HMIApplication::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool HMIApplication::set_appName(const std::string& appName_)
{
  if(appName_.length()>100)  return false;
  appName=appName_;
  return true;
}

bool HMIApplication::set_appType(const std::vector<AppType>& appType_)
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

void HMIApplication::reset_appType(void)
{
  if(appType)
    delete appType;
  appType=0;
}

bool HMIApplication::set_deviceName(const std::string& deviceName_)
{
  deviceName=deviceName_;
  return true;
}

bool HMIApplication::set_hmiDisplayLanguageDesired(const Language& hmiDisplayLanguageDesired_)
{
  if(!LanguageMarshaller::checkIntegrityConst(hmiDisplayLanguageDesired_))   return false;
  hmiDisplayLanguageDesired=hmiDisplayLanguageDesired_;
  return true;
}

bool HMIApplication::set_icon(const std::string& icon_)
{
  delete icon;
  icon=0;

  icon=new std::string(icon_);
  return true;
}

void HMIApplication::reset_icon(void)
{
  if(icon)
    delete icon;
  icon=0;
}

bool HMIApplication::set_isMediaApplication(bool isMediaApplication_)
{
  isMediaApplication=isMediaApplication_;
  return true;
}

bool HMIApplication::set_languageDesired(const Language& languageDesired_)
{
  if(!LanguageMarshaller::checkIntegrityConst(languageDesired_))   return false;
  languageDesired=languageDesired_;
  return true;
}

bool HMIApplication::set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_)
{
  if(ngnMediaScreenAppName_.length()>100)  return false;
  delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;

  ngnMediaScreenAppName=new std::string(ngnMediaScreenAppName_);
  return true;
}

void HMIApplication::reset_ngnMediaScreenAppName(void)
{
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;
}

bool HMIApplication::set_ttsName(const std::vector<TTSChunk>& ttsName_)
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

void HMIApplication::reset_ttsName(void)
{
  if(ttsName)
    delete ttsName;
  ttsName=0;
}

bool HMIApplication::set_vrSynonyms(const std::vector<std::string>& vrSynonyms_)
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

void HMIApplication::reset_vrSynonyms(void)
{
  if(vrSynonyms)
    delete vrSynonyms;
  vrSynonyms=0;
}




int HMIApplication::get_appId(void) const
{
  return appId;
}


const std::string& HMIApplication::get_appName(void) const 
{
  return appName;
}


const std::vector<AppType>* HMIApplication::get_appType(void) const 
{
  return appType;
}


const std::string& HMIApplication::get_deviceName(void) const 
{
  return deviceName;
}


const Language& HMIApplication::get_hmiDisplayLanguageDesired(void) const 
{
  return hmiDisplayLanguageDesired;
}


const std::string* HMIApplication::get_icon(void) const 
{
  return icon;
}


bool HMIApplication::get_isMediaApplication(void) const
{
  return isMediaApplication;
}


const Language& HMIApplication::get_languageDesired(void) const 
{
  return languageDesired;
}


const std::string* HMIApplication::get_ngnMediaScreenAppName(void) const 
{
  return ngnMediaScreenAppName;
}


const std::vector<TTSChunk>* HMIApplication::get_ttsName(void) const 
{
  return ttsName;
}


const std::vector<std::string>* HMIApplication::get_vrSynonyms(void) const 
{
  return vrSynonyms;
}


