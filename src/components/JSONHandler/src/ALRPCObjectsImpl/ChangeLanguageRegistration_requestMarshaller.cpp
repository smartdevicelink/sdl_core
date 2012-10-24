#include "../../include/JSONHandler/ALRPCObjects/ChangeLanguageRegistration_request.h"
#include "LanguageMarshaller.h"

#include "ChangeLanguageRegistration_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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
  if(s.language && !LanguageMarshaller::checkIntegrityConst(*s.language))  return false;
  if(s.hmiDisplayLanguage && !LanguageMarshaller::checkIntegrityConst(*s.hmiDisplayLanguage))  return false;
  return true;
}

Json::Value ChangeLanguageRegistration_requestMarshaller::toJSON(const ChangeLanguageRegistration_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("ChangeLanguageRegistration");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.language)
    j["language"]=LanguageMarshaller::toJSON(*e.language);

  if(e.hmiDisplayLanguage)
    j["hmiDisplayLanguage"]=LanguageMarshaller::toJSON(*e.hmiDisplayLanguage);

  json["request"]["parameters"]=j;
  return json;
}


bool ChangeLanguageRegistration_requestMarshaller::fromJSON(const Json::Value& js,ChangeLanguageRegistration_request& c)
{
  if(c.language)  delete c.language;
  c.language=0;

  if(c.hmiDisplayLanguage)  delete c.hmiDisplayLanguage;
  c.hmiDisplayLanguage=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("ChangeLanguageRegistration"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("language"))
    {
      const Json::Value& j=json["language"];
      c.language=new Language();
      if(!LanguageMarshaller::fromJSON(j,c.language[0]))
        return false;
    }
    if(json.isMember("hmiDisplayLanguage"))
    {
      const Json::Value& j=json["hmiDisplayLanguage"];
      c.hmiDisplayLanguage=new Language();
      if(!LanguageMarshaller::fromJSON(j,c.hmiDisplayLanguage[0]))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

