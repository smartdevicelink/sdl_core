#include "../../include/JSONHandler/ALRPCObjects/OnLanguageChange.h"
#include "OnLanguageChangeMarshaller.h"
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


OnLanguageChange::~OnLanguageChange(void)
{
}


OnLanguageChange::OnLanguageChange(const OnLanguageChange& c)
{
  *this=c;
}


bool OnLanguageChange::checkIntegrity(void)
{
  return OnLanguageChangeMarshaller::checkIntegrity(*this);
}


OnLanguageChange::OnLanguageChange(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONLANGUAGECHANGE)
{
}



bool OnLanguageChange::set_language(const Language& language_)
{
  if(!LanguageMarshaller::checkIntegrityConst(language_))   return false;
  language=language_;
  return true;
}

bool OnLanguageChange::set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_)
{
  if(!LanguageMarshaller::checkIntegrityConst(hmiDisplayLanguage_))   return false;
  hmiDisplayLanguage=hmiDisplayLanguage_;
  return true;
}




const Language& OnLanguageChange::get_language(void) const 
{
  return language;
}

const Language& OnLanguageChange::get_hmiDisplayLanguage(void) const 
{
  return hmiDisplayLanguage;
}

