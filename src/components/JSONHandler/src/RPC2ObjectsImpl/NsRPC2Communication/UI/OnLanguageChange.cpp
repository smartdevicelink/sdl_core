#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnLanguageChange.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnLanguageChange& OnLanguageChange::operator =(const OnLanguageChange& c)
{
  hmiDisplayLanguage=c.hmiDisplayLanguage;
  appId=c.appId;
  return *this;
}


OnLanguageChange::~OnLanguageChange(void)
{
}


OnLanguageChange::OnLanguageChange(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONLANGUAGECHANGE)
{
}


OnLanguageChange::OnLanguageChange(const OnLanguageChange& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONLANGUAGECHANGE)
{
  *this=c;
}


const NsAppLinkRPC::Language& OnLanguageChange::get_hmiDisplayLanguage(void)
{
  return hmiDisplayLanguage;
}

bool OnLanguageChange::set_hmiDisplayLanguage(const NsAppLinkRPC::Language& hmiDisplayLanguage_)
{
  hmiDisplayLanguage=hmiDisplayLanguage_;
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
