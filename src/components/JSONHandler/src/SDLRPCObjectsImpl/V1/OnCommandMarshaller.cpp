//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/OnCommand.h"
#include "TriggerSourceMarshaller.h"

#include "OnCommandMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;


bool OnCommandMarshaller::checkIntegrity(OnCommand& s)
{
  return checkIntegrityConst(s);
}


bool OnCommandMarshaller::fromString(const std::string& s,OnCommand& e)
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


const std::string OnCommandMarshaller::toString(const OnCommand& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnCommandMarshaller::checkIntegrityConst(const OnCommand& s)
{
  if(s.cmdID>2000000000)  return false;
  if(!TriggerSourceMarshaller::checkIntegrityConst(s.triggerSource))  return false;
  return true;
}

Json::Value OnCommandMarshaller::toJSON(const OnCommand& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnCommand");

  Json::Value j=Json::Value(Json::objectValue);

  j["cmdID"]=Json::Value(e.cmdID);

  j["triggerSource"]=TriggerSourceMarshaller::toJSON(e.triggerSource);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnCommandMarshaller::fromJSON(const Json::Value& js,OnCommand& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnCommand"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("cmdID"))  return false;
    {
      const Json::Value& j=json["cmdID"];
      if(!j.isInt())  return false;
      c.cmdID=j.asInt();
    }
    if(!json.isMember("triggerSource"))  return false;
    {
      const Json::Value& j=json["triggerSource"];
      if(!TriggerSourceMarshaller::fromJSON(j,c.triggerSource))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

