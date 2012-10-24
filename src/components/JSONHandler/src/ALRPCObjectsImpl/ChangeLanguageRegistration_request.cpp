#include "../../include/JSONHandler/ALRPCObjects/ChangeLanguageRegistration_request.h"
#include "ChangeLanguageRegistration_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "LanguageMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

ChangeLanguageRegistration_request& ChangeLanguageRegistration_request::operator =(const ChangeLanguageRegistration_request& c)
{
  language= c.language ? new Language(c.language[0]) : 0;
  hmiDisplayLanguage= c.hmiDisplayLanguage ? new Language(c.hmiDisplayLanguage[0]) : 0;

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



bool ChangeLanguageRegistration_request::set_language(const Language& language_)
{
  if(!LanguageMarshaller::checkIntegrityConst(language_))   return false;
  delete language;
  language=0;

  language=new Language(language_);
  return true;
}

void ChangeLanguageRegistration_request::reset_language(void)
{
  if(language)
    delete language;
  language=0;
}

bool ChangeLanguageRegistration_request::set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_)
{
  if(!LanguageMarshaller::checkIntegrityConst(hmiDisplayLanguage_))   return false;
  delete hmiDisplayLanguage;
  hmiDisplayLanguage=0;

  hmiDisplayLanguage=new Language(hmiDisplayLanguage_);
  return true;
}

void ChangeLanguageRegistration_request::reset_hmiDisplayLanguage(void)
{
  if(hmiDisplayLanguage)
    delete hmiDisplayLanguage;
  hmiDisplayLanguage=0;
}




const Language* ChangeLanguageRegistration_request::get_language(void) const 
{
  return language;
}

const Language* ChangeLanguageRegistration_request::get_hmiDisplayLanguage(void) const 
{
  return hmiDisplayLanguage;
}

