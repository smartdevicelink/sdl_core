#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/ChangeRegistration.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VR;


ChangeRegistration& ChangeRegistration::operator =(const ChangeRegistration& c)
{
  language=c.language;
  appId=c.appId;
  return *this;
}


ChangeRegistration::~ChangeRegistration(void)
{
}


ChangeRegistration::ChangeRegistration(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__CHANGEREGISTRATION)
{
}


ChangeRegistration::ChangeRegistration(const ChangeRegistration& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__CHANGEREGISTRATION,c.getId())
{
  *this=c;
}


const NsAppLinkRPCV2::Language& ChangeRegistration::get_language(void)
{
  return language;
}

bool ChangeRegistration::set_language(const NsAppLinkRPCV2::Language& language_)
{
  language=language_;
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
