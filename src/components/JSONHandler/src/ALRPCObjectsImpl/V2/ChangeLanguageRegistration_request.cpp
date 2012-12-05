#include "../include/JSONHandler/ALRPCObjects/V2/ChangeLanguageRegistration_request.h"
#include "ChangeLanguageRegistration_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "LanguageMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

ChangeLanguageRegistration_request::~ChangeLanguageRegistration_request(void)
{
}


ChangeLanguageRegistration_request::ChangeLanguageRegistration_request(const ChangeLanguageRegistration_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool ChangeLanguageRegistration_request::checkIntegrity(void)
{
  return ChangeLanguageRegistration_requestMarshaller::checkIntegrity(*this);
}


ChangeLanguageRegistration_request::ChangeLanguageRegistration_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool ChangeLanguageRegistration_request::set_language(const Language& language_)
{
  if(!LanguageMarshaller::checkIntegrityConst(language_))   return false;
  language=language_;
  return true;
}

bool ChangeLanguageRegistration_request::set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_)
{
  if(!LanguageMarshaller::checkIntegrityConst(hmiDisplayLanguage_))   return false;
  hmiDisplayLanguage=hmiDisplayLanguage_;
  return true;
}




const Language& ChangeLanguageRegistration_request::get_language(void) const 
{
  return language;
}

const Language& ChangeLanguageRegistration_request::get_hmiDisplayLanguage(void) const 
{
  return hmiDisplayLanguage;
}

