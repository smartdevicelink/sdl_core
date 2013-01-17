#include "../include/JSONHandler/ALRPCObjects/V2/OnLanguageChange.h"
#include "LanguageMarshaller.h"

#include "OnLanguageChangeMarshaller.h"

#include <iostream>
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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
  std::cout<<"OnLanguageChangeMarshaller::checkIntegrityConst: " << s.language.get() << "\n";
  if(!LanguageMarshaller::checkIntegrityConst(s.language))  return false;
  std::cout<<"OnLanguageChangeMarshaller::checkIntegrityConst\n";
  if(!LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguage))  return false;
  return true;
}

Json::Value OnLanguageChangeMarshaller::toJSON(const OnLanguageChange& e)
{
  Json::Value json(Json::objectValue);
  std::cout<<"\t\tOnLanguageChangeMarshaller::toJSON\n";
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  std::cout<<"\t\tOnLanguageChangeMarshaller::toJSON:: integrity.\n";

  json["language"]=LanguageMarshaller::toJSON(e.language);

  json["hmiDisplayLanguage"]=LanguageMarshaller::toJSON(e.hmiDisplayLanguage);

  return json;
}


bool OnLanguageChangeMarshaller::fromJSON(const Json::Value& json,OnLanguageChange& c)
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

