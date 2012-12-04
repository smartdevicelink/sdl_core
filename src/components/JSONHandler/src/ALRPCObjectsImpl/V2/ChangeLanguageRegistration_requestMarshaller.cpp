#include "../include/JSONHandler/ALRPCObjects/V2/ChangeLanguageRegistration_request.h"
#include "LanguageMarshaller.h"

#include "ChangeLanguageRegistration_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool ChangeLanguageRegistration_requestMarshaller::checkIntegrity(ChangeLanguageRegistration_request& s)
{
  return checkIntegrityConst(s);
}


bool ChangeLanguageRegistration_requestMarshaller::fromString(const std::string& s,ChangeLanguageRegistration_request& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string ChangeLanguageRegistration_requestMarshaller::toString(const ChangeLanguageRegistration_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ChangeLanguageRegistration_requestMarshaller::checkIntegrityConst(const ChangeLanguageRegistration_request& s)
{
  if(!LanguageMarshaller::checkIntegrityConst(s.language))  return false;
  if(!LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguage))  return false;
  return true;
}

Json::Value ChangeLanguageRegistration_requestMarshaller::toJSON(const ChangeLanguageRegistration_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["language"]=LanguageMarshaller::toJSON(e.language);

  json["hmiDisplayLanguage"]=LanguageMarshaller::toJSON(e.hmiDisplayLanguage);

  return json;
}


bool ChangeLanguageRegistration_requestMarshaller::fromJSON(const Json::Value& json,ChangeLanguageRegistration_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("language"))  return false;
    {
      const Json::Value& j=json["language"];
      if(!LanguageMarshaller::fromJSON(j,c.language))
        return false;
    }
    if(!json.isMember("hmiDisplayLanguage"))  return false;
    {
      const Json::Value& j=json["hmiDisplayLanguage"];
      if(!LanguageMarshaller::fromJSON(j,c.hmiDisplayLanguage))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

