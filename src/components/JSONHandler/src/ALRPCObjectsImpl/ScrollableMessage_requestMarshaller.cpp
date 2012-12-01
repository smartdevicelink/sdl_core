#include "../include/JSONHandler/ALRPCObjects/ScrollableMessage_request.h"
#include "SoftButtonMarshaller.h"

#include "ScrollableMessage_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool ScrollableMessage_requestMarshaller::checkIntegrity(ScrollableMessage_request& s)
{
  return checkIntegrityConst(s);
}


bool ScrollableMessage_requestMarshaller::fromString(const std::string& s,ScrollableMessage_request& e)
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


const std::string ScrollableMessage_requestMarshaller::toString(const ScrollableMessage_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ScrollableMessage_requestMarshaller::checkIntegrityConst(const ScrollableMessage_request& s)
{
  if(s.scrollableMessageBody.length()>500)  return false;
  if(s.timeout && *s.timeout>65535)  return false;
  if(s.softButtons)
  {
    unsigned int i=s.softButtons[0].size();
    if(i>8 || i<0)  return false;
    while(i--)
    {
    if(!SoftButtonMarshaller::checkIntegrityConst(s.softButtons[0][i]))   return false;
    }
  }
  return true;
}

Json::Value ScrollableMessage_requestMarshaller::toJSON(const ScrollableMessage_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("ScrollableMessage");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["scrollableMessageBody"]=Json::Value(e.scrollableMessageBody);

  if(e.timeout)
    j["timeout"]=Json::Value(*e.timeout);

  if(e.softButtons)
  {
    unsigned int sz=e.softButtons->size();
    j["softButtons"]=Json::Value(Json::arrayValue);
    j["softButtons"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[0][i]);
  }

  json["request"]["parameters"]=j;
  return json;
}


bool ScrollableMessage_requestMarshaller::fromJSON(const Json::Value& js,ScrollableMessage_request& c)
{
  if(c.timeout)  delete c.timeout;
  c.timeout=0;

  if(c.softButtons)  delete c.softButtons;
  c.softButtons=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("ScrollableMessage"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("scrollableMessageBody"))  return false;
    {
      const Json::Value& j=json["scrollableMessageBody"];
      if(!j.isString())  return false;
      c.scrollableMessageBody=j.asString();
    }
    if(json.isMember("timeout"))
    {
      const Json::Value& j=json["timeout"];
      if(!j.isInt())  return false;
      c.timeout=new unsigned int(j.asInt());
    }
    if(json.isMember("softButtons"))
    {
      const Json::Value& j=json["softButtons"];
      if(!j.isArray())  return false;
      c.softButtons=new std::vector<SoftButton>();
      c.softButtons->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        SoftButton t;
        if(!SoftButtonMarshaller::fromJSON(j[i],t))
          return false;
        c.softButtons[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

