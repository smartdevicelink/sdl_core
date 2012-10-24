#include "../../include/JSONHandler/ALRPCObjects/OnLanguageChange.h"
#include "LanguageMarshaller.h"

#include "OnLanguageChangeMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool OnLanguageChangeMarshaller::checkIntegrity(OnLanguageChange& s)
{
  return checkIntegrityConst(s);
}


bool OnLanguageChangeMarshaller::fromString(const std::string& s,OnLanguageChange& e)
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


const std::string OnLanguageChangeMarshaller::toString(const OnLanguageChange& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnLanguageChangeMarshaller::checkIntegrityConst(const OnLanguageChange& s)
{
  if(!LanguageMarshaller::checkIntegrityConst(s.language))  return false;
  if(!LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguage))  return false;
  return true;
}

Json::Value OnLanguageChangeMarshaller::toJSON(const OnLanguageChange& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnLanguageChange");

  Json::Value j=Json::Value(Json::objectValue);

  j["language"]=LanguageMarshaller::toJSON(e.language);

  j["hmiDisplayLanguage"]=LanguageMarshaller::toJSON(e.hmiDisplayLanguage);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnLanguageChangeMarshaller::fromJSON(const Json::Value& js,OnLanguageChange& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnLanguageChange"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
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

