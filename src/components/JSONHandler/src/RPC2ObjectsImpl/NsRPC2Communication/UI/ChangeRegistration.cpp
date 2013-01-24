#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ChangeRegistration.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


ChangeRegistration& ChangeRegistration::operator =(const ChangeRegistration& c)
{
  hmiDisplayLanguage=c.hmiDisplayLanguage;
  appId=c.appId;
  return *this;
}


ChangeRegistration::~ChangeRegistration(void)
{
}


ChangeRegistration::ChangeRegistration(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATION)
{
}


ChangeRegistration::ChangeRegistration(const ChangeRegistration& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATION,c.getId())
{
  *this=c;
}


const NsAppLinkRPCV2::Language& ChangeRegistration::get_hmiDisplayLanguage(void)
{
  return hmiDisplayLanguage;
}

bool ChangeRegistration::set_hmiDisplayLanguage(const NsAppLinkRPCV2::Language& hmiDisplayLanguage_)
{
  hmiDisplayLanguage=hmiDisplayLanguage_;
  return true;
}

int ChangeRegistration::get_appId(void)
{
  return appId;
}

bool ChangeRegistration::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ChangeRegistration::checkIntegrity(void)
{
  return ChangeRegistrationMarshaller::checkIntegrity(*this);
}
