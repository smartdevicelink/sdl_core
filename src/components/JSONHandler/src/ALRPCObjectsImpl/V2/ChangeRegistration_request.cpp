#include "../include/JSONHandler/ALRPCObjects/V2/ChangeRegistration_request.h"
#include "ChangeRegistration_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "LanguageMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

ChangeRegistration_request::~ChangeRegistration_request(void)
{
}


ChangeRegistration_request::ChangeRegistration_request(const ChangeRegistration_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool ChangeRegistration_request::checkIntegrity(void)
{
  return ChangeRegistration_requestMarshaller::checkIntegrity(*this);
}


ChangeRegistration_request::ChangeRegistration_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool ChangeRegistration_request::set_language(const Language& language_)
{
  if(!LanguageMarshaller::checkIntegrityConst(language_))   return false;
  language=language_;
  return true;
}

bool ChangeRegistration_request::set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_)
{
  if(!LanguageMarshaller::checkIntegrityConst(hmiDisplayLanguage_))   return false;
  hmiDisplayLanguage=hmiDisplayLanguage_;
  return true;
}




const Language& ChangeRegistration_request::get_language(void) const 
{
  return language;
}

const Language& ChangeRegistration_request::get_hmiDisplayLanguage(void) const 
{
  return hmiDisplayLanguage;
}

