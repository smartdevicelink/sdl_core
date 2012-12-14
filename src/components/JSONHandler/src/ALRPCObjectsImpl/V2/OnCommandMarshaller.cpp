#include "../include/JSONHandler/ALRPCObjects/V2/OnCommand.h"
#include "TriggerSourceMarshaller.h"

#include "OnCommandMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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

  json["cmdID"]=Json::Value(e.cmdID);

  json["triggerSource"]=TriggerSourceMarshaller::toJSON(e.triggerSource);

  return json;
}


bool OnCommandMarshaller::fromJSON(const Json::Value& json,OnCommand& c)
{
  try
  {
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

