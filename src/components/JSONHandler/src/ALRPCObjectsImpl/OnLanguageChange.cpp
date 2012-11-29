#include "../include/JSONHandler/ALRPCObjects/OnLanguageChange.h"
#include "OnLanguageChangeMarshaller.h"
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



bool OnLanguageChange::set_language(const Language_v2& language_)
{
  if(!Language_v2Marshaller::checkIntegrityConst(language_))   return false;
  language=language_;
  return true;
}

bool OnLanguageChange::set_hmiDisplayLanguage(const Language_v2& hmiDisplayLanguage_)
{
  if(!Language_v2Marshaller::checkIntegrityConst(hmiDisplayLanguage_))   return false;
  hmiDisplayLanguage=hmiDisplayLanguage_;
  return true;
}




const Language_v2& OnLanguageChange::get_language(void) const 
{
  return language;
}

const Language_v2& OnLanguageChange::get_hmiDisplayLanguage(void) const 
{
  return hmiDisplayLanguage;
}

