#include "../include/JSONHandler/ALRPCObjects/V1/HMIApplication.h"


#include "HMIApplicationMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool HMIApplicationMarshaller::checkIntegrity(HMIApplication& s)
{
  return checkIntegrityConst(s);
}


bool HMIApplicationMarshaller::fromString(const std::string& s,HMIApplication& e)
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


const std::string HMIApplicationMarshaller::toString(const HMIApplication& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool HMIApplicationMarshaller::checkIntegrityConst(const HMIApplication& s)
{
  if(s.appName.length()>100)  return false;
  if(s.ngnMediaScreenAppName && s.ngnMediaScreenAppName->length()>100)  return false;
  return true;
}

Json::Value HMIApplicationMarshaller::toJSON(const HMIApplication& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["appName"]=Json::Value(e.appName);

  if(e.icon)
    json["icon"]=Json::Value(*e.icon);

  if(e.ngnMediaScreenAppName)
    json["ngnMediaScreenAppName"]=Json::Value(*e.ngnMediaScreenAppName);

  json["isMediaApplication"]=Json::Value(e.mIsMediaApplication);

  //if (e.appId)
  json["appId"] = Json::Value(e.appId);


  return json;
}


bool HMIApplicationMarshaller::fromJSON(const Json::Value& json,HMIApplication& c)
{
  if(c.icon)  delete c.icon;
  c.icon=0;

  if(c.ngnMediaScreenAppName)  delete c.ngnMediaScreenAppName;
  c.ngnMediaScreenAppName=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("appName"))  return false;
    {
      const Json::Value& j=json["appName"];
      if(!j.isString())  return false;
      c.appName=j.asString();
    }
    if(!json.isMember("appId")) return false;
    {
      const Json::Value j = json["appId"];
      if (!j.isInt()) return false;

      c.appId = j.asInt();
    }
    if(json.isMember("icon"))
    {
      const Json::Value& j=json["icon"];
      if(!j.isString())  return false;
      c.icon=new std::string(j.asString());
    }
    if(json.isMember("ngnMediaScreenAppName"))
    {
      const Json::Value& j=json["ngnMediaScreenAppName"];
      if(!j.isString())  return false;
      c.ngnMediaScreenAppName=new std::string(j.asString());
    }
    if(json.isMember("isMediaApplication"))
    {
      const Json::Value& j=json["isMediaApplication"];
 //     if(!j.isString())  return false;
      c.mIsMediaApplication=j.asBool();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

