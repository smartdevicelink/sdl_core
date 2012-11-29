#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/OnLanguageChange.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VR;


OnLanguageChange& OnLanguageChange::operator =(const OnLanguageChange& c)
{
  language=c.language;
  appId=c.appId;
  return *this;
}


OnLanguageChange::~OnLanguageChange(void)
{
}


OnLanguageChange::OnLanguageChange(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ONLANGUAGECHANGE)
{
}


OnLanguageChange::OnLanguageChange(const OnLanguageChange& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ONLANGUAGECHANGE)
{
  *this=c;
}


const NsAppLinkRPC::Language& OnLanguageChange::get_language(void)
{
  return language;
}

bool OnLanguageChange::set_language(const NsAppLinkRPC::Language& language_)
{
  language=language_;
  return true;
}

int OnLanguageChange::get_appId(void)
{
  return appId;
}

bool OnLanguageChange::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool OnLanguageChange::checkIntegrity(void)
{
  return OnLanguageChangeMarshaller::checkIntegrity(*this);
}
