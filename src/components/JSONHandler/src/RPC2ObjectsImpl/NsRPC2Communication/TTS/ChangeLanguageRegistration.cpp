#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/ChangeLanguageRegistration.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;


ChangeLanguageRegistration& ChangeLanguageRegistration::operator =(const ChangeLanguageRegistration& c)
{
  language=c.language;
  appId=c.appId;
  return *this;
}


ChangeLanguageRegistration::~ChangeLanguageRegistration(void)
{
}


ChangeLanguageRegistration::ChangeLanguageRegistration(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__CHANGELANGUAGEREGISTRATION)
{
}


ChangeLanguageRegistration::ChangeLanguageRegistration(const ChangeLanguageRegistration& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__CHANGELANGUAGEREGISTRATION,c.getId())
{
  *this=c;
}


const NsAppLinkRPC::Language& ChangeLanguageRegistration::get_language(void)
{
  return language;
}

bool ChangeLanguageRegistration::set_language(const NsAppLinkRPC::Language& language_)
{
  language=language_;
  return true;
}

int ChangeLanguageRegistration::get_appId(void)
{
  return appId;
}

bool ChangeLanguageRegistration::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ChangeLanguageRegistration::checkIntegrity(void)
{
  return ChangeLanguageRegistrationMarshaller::checkIntegrity(*this);
}
