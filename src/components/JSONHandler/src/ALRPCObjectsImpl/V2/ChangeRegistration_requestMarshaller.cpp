#include "../include/JSONHandler/ALRPCObjects/V2/ChangeRegistration_request.h"
#include "LanguageMarshaller.h"

#include "ChangeRegistration_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool ChangeRegistration_requestMarshaller::checkIntegrity(ChangeRegistration_request& s)
{
  return checkIntegrityConst(s);
}


bool ChangeRegistration_requestMarshaller::fromString(const std::string& s,ChangeRegistration_request& e)
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


const std::string ChangeRegistration_requestMarshaller::toString(const ChangeRegistration_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ChangeRegistration_requestMarshaller::checkIntegrityConst(const ChangeRegistration_request& s)
{
  if(!LanguageMarshaller::checkIntegrityConst(s.language))  return false;
  if(!LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguage))  return false;
  return true;
}

Json::Value ChangeRegistration_requestMarshaller::toJSON(const ChangeRegistration_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["language"]=LanguageMarshaller::toJSON(e.language);

  json["hmiDisplayLanguage"]=LanguageMarshaller::toJSON(e.hmiDisplayLanguage);

  return json;
}


bool ChangeRegistration_requestMarshaller::fromJSON(const Json::Value& json,ChangeRegistration_request& c)
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

