#include "../include/JSONHandler/ALRPCObjects/V2/ResetGlobalProperties_request.h"
#include "GlobalPropertyMarshaller.h"

#include "ResetGlobalProperties_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool ResetGlobalProperties_requestMarshaller::checkIntegrity(ResetGlobalProperties_request& s)
{
  return checkIntegrityConst(s);
}


bool ResetGlobalProperties_requestMarshaller::fromString(const std::string& s,ResetGlobalProperties_request& e)
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


const std::string ResetGlobalProperties_requestMarshaller::toString(const ResetGlobalProperties_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ResetGlobalProperties_requestMarshaller::checkIntegrityConst(const ResetGlobalProperties_request& s)
{
  {
    unsigned int i=s.properties.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!GlobalPropertyMarshaller::checkIntegrityConst(s.properties[i]))   return false;
    }
  }
  return true;
}

Json::Value ResetGlobalProperties_requestMarshaller::toJSON(const ResetGlobalProperties_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["properties"]=Json::Value(Json::arrayValue);
  json["properties"].resize(e.properties.size());
  for(unsigned int i=0;i<e.properties.size();i++)
    json["properties"][i]=GlobalPropertyMarshaller::toJSON(e.properties[i]);

  return json;
}


bool ResetGlobalProperties_requestMarshaller::fromJSON(const Json::Value& json,ResetGlobalProperties_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("properties"))  return false;
    {
      const Json::Value& j=json["properties"];
      if(!j.isArray())  return false;
      c.properties.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          GlobalProperty t;
          if(!GlobalPropertyMarshaller::fromJSON(j[i],t))
            return false;
          c.properties[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

