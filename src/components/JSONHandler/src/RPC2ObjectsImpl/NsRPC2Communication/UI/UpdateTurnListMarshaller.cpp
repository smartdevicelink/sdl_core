#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/UpdateTurnList.h"
#include "../src/ALRPCObjectsImpl/V2/TurnMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SoftButtonMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/UpdateTurnListMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool UpdateTurnListMarshaller::checkIntegrity(UpdateTurnList& s)
{
  return checkIntegrityConst(s);
}


bool UpdateTurnListMarshaller::fromString(const std::string& s,UpdateTurnList& e)
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


const std::string UpdateTurnListMarshaller::toString(const UpdateTurnList& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UpdateTurnListMarshaller::checkIntegrityConst(const UpdateTurnList& s)
{
  {
    unsigned int i=s.turnList.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  {
    unsigned int i=s.softButtons.size();
    if(i<0)  return false;
    if(i>1)  return false;
  }

  return true;
}


Json::Value UpdateTurnListMarshaller::toJSON(const UpdateTurnList& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.UpdateTurnList");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  {
    unsigned int i=e.turnList.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::TurnMarshaller::toJSON(e.turnList[i]);

    json["params"]["turnList"]=j;
  }
  {
    unsigned int i=e.softButtons.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::SoftButtonMarshaller::toJSON(e.softButtons[i]);

    json["params"]["softButtons"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool UpdateTurnListMarshaller::fromJSON(const Json::Value& json,UpdateTurnList& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.UpdateTurnList"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("turnList") || !js["turnList"].isArray())  return false;
    {
      unsigned int i=js["turnList"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.turnList.resize(i);
      while(i--)
      {
        NsAppLinkRPCV2::Turn t;
        if(!NsAppLinkRPCV2::TurnMarshaller::fromJSON(js["turnList"][i],t))
          return false;
         c.turnList[i]=t;
      }
    }

    if(!js.isMember("softButtons") || !js["softButtons"].isArray())  return false;
    {
      unsigned int i=js["softButtons"].size();
      if(i<0)  return false;
      if(i>1)  return false;
      c.softButtons.resize(i);
      while(i--)
      {
        NsAppLinkRPCV2::SoftButton t;
        if(!NsAppLinkRPCV2::SoftButtonMarshaller::fromJSON(js["softButtons"][i],t))
          return false;
         c.softButtons[i]=t;
      }
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
