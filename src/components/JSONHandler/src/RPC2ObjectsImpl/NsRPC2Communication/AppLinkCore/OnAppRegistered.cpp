#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppRegistered.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
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
  return *this;
}


OnAppRegistered::~OnAppRegistered(void)
{
  if(vrSynonym)  delete vrSynonym;
}


OnAppRegistered::OnAppRegistered(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED),
  vrSynonym(0)
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

bool OnAppRegistered::checkIntegrity(void)
{
  return OnAppRegisteredMarshaller::checkIntegrity(*this);
}
