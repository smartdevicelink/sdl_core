#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ScrollableMessage.h"
#include "../src/ALRPCObjectsImpl/V2/SoftButtonMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ScrollableMessageMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool ScrollableMessageMarshaller::checkIntegrity(ScrollableMessage& s)
{
  return checkIntegrityConst(s);
}


bool ScrollableMessageMarshaller::fromString(const std::string& s,ScrollableMessage& e)
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


const std::string ScrollableMessageMarshaller::toString(const ScrollableMessage& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ScrollableMessageMarshaller::checkIntegrityConst(const ScrollableMessage& s)
{
  if(s.scrollableMessageBody.length()>500)  return false;

  if(s.timeout && (s.timeout[0]>65535))  return false;

  if(s.softButtons)
  {
    unsigned int i=s.softButtons[0].size();
    if(i<0)  return false;
    if(i>8)  return false;
  }

  return true;
}


Json::Value ScrollableMessageMarshaller::toJSON(const ScrollableMessage& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.ScrollableMessage");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["scrollableMessageBody"]=Json::Value(e.scrollableMessageBody);;
  if(e.timeout)
    json["params"]["timeout"]=Json::Value(e.timeout[0]);;
  if(e.softButtons)
  {
    unsigned int i=e.softButtons[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::SoftButtonMarshaller::toJSON(e.softButtons[0][i]);

    json["params"]["softButtons"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool ScrollableMessageMarshaller::fromJSON(const Json::Value& json,ScrollableMessage& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.ScrollableMessage"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("scrollableMessageBody") || !js["scrollableMessageBody"].isString())  return false;
    c.scrollableMessageBody=js["scrollableMessageBody"].asString();
    if(c.scrollableMessageBody.length()>500)  return false;

    if(c.timeout)  delete c.timeout;
    c.timeout=0;
    if(js.isMember("timeout"))
    {
      if(!js["timeout"].isInt())  return false;
      c.timeout=new unsigned int();
      c.timeout[0]=js["timeout"].asInt();
      if(c.timeout[0]>65535)  return false;

    }

    if(c.softButtons)  delete c.softButtons;
    c.softButtons=0;
    if(js.isMember("softButtons"))
    {
      if(!js["softButtons"].isArray()) return false;
      unsigned int i=js["softButtons"].size();
      if(i<0)  return false;
      if(i>8)  return false;

      c.softButtons=new std::vector<NsAppLinkRPCV2::SoftButton>();
      c.softButtons->resize(js["softButtons"].size());

      while(i--)
        if(!NsAppLinkRPCV2::SoftButtonMarshaller::fromJSON(js["softButtons"][i],c.softButtons[0][i]))  return false;
    }


    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
