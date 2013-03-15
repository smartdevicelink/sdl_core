//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/OnLanguageChange.h"
#include "OnLanguageChangeMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "LanguageMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;

OnLanguageChange::~OnLanguageChange(void)
{
}


OnLanguageChange::OnLanguageChange(const OnLanguageChange& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool OnLanguageChange::checkIntegrity(void)
{
  return OnLanguageChangeMarshaller::checkIntegrity(*this);
}


OnLanguageChange::OnLanguageChange(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
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

