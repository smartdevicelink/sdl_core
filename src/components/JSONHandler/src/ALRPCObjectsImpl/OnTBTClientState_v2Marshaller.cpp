#include "../include/JSONHandler/ALRPCObjects/OnTBTClientState_v2.h"
#include "TBTState_v2Marshaller.h"

#include "OnTBTClientState_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool OnTBTClientState_v2Marshaller::checkIntegrity(OnTBTClientState_v2& s)
{
  return checkIntegrityConst(s);
}


bool OnTBTClientState_v2Marshaller::fromString(const std::string& s,OnTBTClientState_v2& e)
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


const std::string OnTBTClientState_v2Marshaller::toString(const OnTBTClientState_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnTBTClientState_v2Marshaller::checkIntegrityConst(const OnTBTClientState_v2& s)
{
  if(!TBTState_v2Marshaller::checkIntegrityConst(s.state))  return false;
  return true;
}

Json::Value OnTBTClientState_v2Marshaller::toJSON(const OnTBTClientState_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnTBTClientState_v2");

  Json::Value j=Json::Value(Json::objectValue);

  j["state"]=TBTState_v2Marshaller::toJSON(e.state);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnTBTClientState_v2Marshaller::fromJSON(const Json::Value& js,OnTBTClientState_v2& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnTBTClientState_v2"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("state"))  return false;
    {
      const Json::Value& j=json["state"];
      if(!TBTState_v2Marshaller::fromJSON(j,c.state))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

