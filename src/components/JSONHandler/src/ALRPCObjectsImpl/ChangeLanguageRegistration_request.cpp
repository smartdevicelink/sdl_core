#include "../include/JSONHandler/ALRPCObjects/ChangeLanguageRegistration_request.h"
#include "ChangeLanguageRegistration_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "Language_v2Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
ChangeLanguageRegistration_request& ChangeLanguageRegistration_request::operator =(const ChangeLanguageRegistration_request& c)
{
  language= c.language ? new Language_v2(c.language[0]) : 0;
  hmiDisplayLanguage= c.hmiDisplayLanguage ? new Language_v2(c.hmiDisplayLanguage[0]) : 0;

  return *this;}


ChangeLanguageRegistration_request::~ChangeLanguageRegistration_request(void)
{
  if(language)
    delete language;
  if(hmiDisplayLanguage)
    delete hmiDisplayLanguage;
}


ChangeLanguageRegistration_request::ChangeLanguageRegistration_request(const ChangeLanguageRegistration_request& c)
{
  *this=c;
}


bool ChangeLanguageRegistration_request::checkIntegrity(void)
{
  return ChangeLanguageRegistration_requestMarshaller::checkIntegrity(*this);
}


ChangeLanguageRegistration_request::ChangeLanguageRegistration_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_CHANGELANGUAGEREGISTRATION_REQUEST),
      language(0),
    hmiDisplayLanguage(0)
{
}



bool ChangeLanguageRegistration_request::set_language(const Language_v2& language_)
{
  if(!Language_v2Marshaller::checkIntegrityConst(language_))   return false;
  delete language;
  language=0;

  language=new Language_v2(language_);
  return true;
}

void ChangeLanguageRegistration_request::reset_language(void)
{
  if(language)
    delete language;
  language=0;
}

bool ChangeLanguageRegistration_request::set_hmiDisplayLanguage(const Language_v2& hmiDisplayLanguage_)
{
  if(!Language_v2Marshaller::checkIntegrityConst(hmiDisplayLanguage_))   return false;
  delete hmiDisplayLanguage;
  hmiDisplayLanguage=0;

  hmiDisplayLanguage=new Language_v2(hmiDisplayLanguage_);
  return true;
}

void ChangeLanguageRegistration_request::reset_hmiDisplayLanguage(void)
{
  if(hmiDisplayLanguage)
    delete hmiDisplayLanguage;
  hmiDisplayLanguage=0;
}




const Language_v2* ChangeLanguageRegistration_request::get_language(void) const 
{
  return language;
}

const Language_v2* ChangeLanguageRegistration_request::get_hmiDisplayLanguage(void) const 
{
  return hmiDisplayLanguage;
}

