#include "../include/JSONHandler/ALRPCObjects/V2/OnLanguageChange.h"
#include "OnLanguageChangeMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "LanguageMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

OnLanguageChange::~OnLanguageChange(void)
{
}


OnLanguageChange::OnLanguageChange(const OnLanguageChange& c) : ALRPC2Message(c)
{
  *this=c;
}


bool OnLanguageChange::checkIntegrity(void)
{
  return OnLanguageChangeMarshaller::checkIntegrity(*this);
}


OnLanguageChange::OnLanguageChange(void) : ALRPC2Message(PROTOCOL_VERSION)
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

