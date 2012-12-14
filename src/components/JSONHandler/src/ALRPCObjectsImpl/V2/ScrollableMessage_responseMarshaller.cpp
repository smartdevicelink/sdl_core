#include "../include/JSONHandler/ALRPCObjects/V2/ScrollableMessage_response.h"
#include "ResultMarshaller.h"

#include "ScrollableMessage_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool ScrollableMessage_responseMarshaller::checkIntegrity(ScrollableMessage_response& s)
{
  return checkIntegrityConst(s);
}


bool ScrollableMessage_responseMarshaller::fromString(const std::string& s,ScrollableMessage_response& e)
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


const std::string ScrollableMessage_responseMarshaller::toString(const ScrollableMessage_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ScrollableMessage_responseMarshaller::checkIntegrityConst(const ScrollableMessage_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
  return true;
}

Json::Value ScrollableMessage_responseMarshaller::toJSON(const ScrollableMessage_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  return json;
}


bool ScrollableMessage_responseMarshaller::fromJSON(const Json::Value& json,ScrollableMessage_response& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("success"))  return false;
    {
      const Json::Value& j=json["success"];
      if(!j.isBool())  return false;
      c.success=j.asBool();
    }
    if(!json.isMember("resultCode"))  return false;
    {
      const Json::Value& j=json["resultCode"];
      if(!ResultMarshaller::fromJSON(j,c.resultCode))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

