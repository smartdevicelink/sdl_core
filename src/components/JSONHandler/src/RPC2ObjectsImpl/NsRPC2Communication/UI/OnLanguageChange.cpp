#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnLanguageChange.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnLanguageChange& OnLanguageChange::operator =(const OnLanguageChange& c)
{
  hmiDisplayLanguage=c.hmiDisplayLanguage;
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


const NsAppLinkRPCV2::Language& OnLanguageChange::get_hmiDisplayLanguage(void)
{
  return hmiDisplayLanguage;
}

bool OnLanguageChange::set_hmiDisplayLanguage(const NsAppLinkRPCV2::Language& hmiDisplayLanguage_)
{
  hmiDisplayLanguage=hmiDisplayLanguage_;
  return true;
}

bool OnLanguageChange::checkIntegrity(void)
{
  return OnLanguageChangeMarshaller::checkIntegrity(*this);
}
