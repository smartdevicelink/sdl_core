#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppRegistered.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


OnAppRegistered& OnAppRegistered::operator =(const OnAppRegistered& c)
{
  appName=c.appName;
  appIcon=c.appIcon;
  deviceName=c.deviceName;
  if(vrSynonym)  delete vrSynonym;
  vrSynonym= c.vrSynonym ? new std::vector<std::string>(c.vrSynonym[0]) : 0;
  isMediaApplication=c.isMediaApplication;
  languageDesired=c.languageDesired;
  hmiDisplayLanguageDesired=c.hmiDisplayLanguageDesired;
  if(ttsName)  delete ttsName;
  ttsName= c.ttsName ? new std::vector<NsAppLinkRPC::TTSChunk>(c.ttsName[0]) : 0;
  if(appType)  delete appType;
  appType= c.appType ? new std::vector<NsAppLinkRPC::AppType>(c.appType[0]) : 0;
  appId=c.appId;
  return *this;
}


OnAppRegistered::~OnAppRegistered(void)
{
  if(vrSynonym)  delete vrSynonym;
  if(ttsName)  delete ttsName;
  if(appType)  delete appType;
}


OnAppRegistered::OnAppRegistered(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED),
  vrSynonym(0),
  ttsName(0),
  appType(0)
{
}


OnAppRegistered::OnAppRegistered(const OnAppRegistered& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED)
{
  *this=c;
}


const std::string& OnAppRegistered::get_appName(void)
{
  return appName;
}

bool OnAppRegistered::set_appName(const std::string& appName_)
{
  appName=appName_;
  return true;
}

const std::string& OnAppRegistered::get_appIcon(void)
{
  return appIcon;
}

bool OnAppRegistered::set_appIcon(const std::string& appIcon_)
{
  appIcon=appIcon_;
  return true;
}

const std::string& OnAppRegistered::get_deviceName(void)
{
  return deviceName;
}

bool OnAppRegistered::set_deviceName(const std::string& deviceName_)
{
  deviceName=deviceName_;
  return true;
}

const std::vector< std::string>* OnAppRegistered::get_vrSynonym(void)
{
  return vrSynonym;
}

bool OnAppRegistered::set_vrSynonym(const std::vector< std::string>& vrSynonym_)
{
  if(vrSynonym)  delete vrSynonym;
  vrSynonym=new std::vector< std::string>(vrSynonym_);
  return true;
}

void OnAppRegistered::reset_vrSynonym(void)
{
  if(vrSynonym)  delete vrSynonym;
  vrSynonym=0;
}

bool OnAppRegistered::get_isMediaApplication(void)
{
  return isMediaApplication;
}

bool OnAppRegistered::set_isMediaApplication(bool isMediaApplication_)
{
  isMediaApplication=isMediaApplication_;
  return true;
}

const NsAppLinkRPC::Language& OnAppRegistered::get_languageDesired(void)
{
  return languageDesired;
}

bool OnAppRegistered::set_languageDesired(const NsAppLinkRPC::Language& languageDesired_)
{
  languageDesired=languageDesired_;
  return true;
}

const NsAppLinkRPC::Language& OnAppRegistered::get_hmiDisplayLanguageDesired(void)
{
  return hmiDisplayLanguageDesired;
}

bool OnAppRegistered::set_hmiDisplayLanguageDesired(const NsAppLinkRPC::Language& hmiDisplayLanguageDesired_)
{
  hmiDisplayLanguageDesired=hmiDisplayLanguageDesired_;
  return true;
}

const std::vector< NsAppLinkRPC::TTSChunk>* OnAppRegistered::get_ttsName(void)
{
  return ttsName;
}

bool OnAppRegistered::set_ttsName(const std::vector< NsAppLinkRPC::TTSChunk>& ttsName_)
{
  if(ttsName)  delete ttsName;
  ttsName=new std::vector< NsAppLinkRPC::TTSChunk>(ttsName_);
  return true;
}

void OnAppRegistered::reset_ttsName(void)
{
  if(ttsName)  delete ttsName;
  ttsName=0;
}

const std::vector< NsAppLinkRPC::AppType>* OnAppRegistered::get_appType(void)
{
  return appType;
}

bool OnAppRegistered::set_appType(const std::vector< NsAppLinkRPC::AppType>& appType_)
{
  if(appType)  delete appType;
  appType=new std::vector< NsAppLinkRPC::AppType>(appType_);
  return true;
}

void OnAppRegistered::reset_appType(void)
{
  if(appType)  delete appType;
  appType=0;
}

int OnAppRegistered::get_appId(void)
{
  return appId;
}

bool OnAppRegistered::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool OnAppRegistered::checkIntegrity(void)
{
  return OnAppRegisteredMarshaller::checkIntegrity(*this);
}
