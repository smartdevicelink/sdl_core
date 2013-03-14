#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertManeuver.h"
#include "../src/SDLRPCObjectsImpl/V2/TTSChunkMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/SoftButtonMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AlertManeuverMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool AlertManeuverMarshaller::checkIntegrity(AlertManeuver& s)
{
  return checkIntegrityConst(s);
}


bool AlertManeuverMarshaller::fromString(const std::string& s,AlertManeuver& e)
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


const std::string AlertManeuverMarshaller::toString(const AlertManeuver& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AlertManeuverMarshaller::checkIntegrityConst(const AlertManeuver& s)
{
  {
    unsigned int i=s.ttsChunks.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  {
    unsigned int i=s.softButtons.size();
    if(i<0)  return false;
    if(i>3)  return false;
  }

  return true;
}


Json::Value AlertManeuverMarshaller::toJSON(const AlertManeuver& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.AlertManeuver");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  {
    unsigned int i=e.ttsChunks.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::TTSChunkMarshaller::toJSON(e.ttsChunks[i]);

    json["params"]["ttsChunks"]=j;
  }
  {
    unsigned int i=e.softButtons.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::SoftButtonMarshaller::toJSON(e.softButtons[i]);

    json["params"]["softButtons"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool AlertManeuverMarshaller::fromJSON(const Json::Value& json,AlertManeuver& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.AlertManeuver"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("ttsChunks") || !js["ttsChunks"].isArray())  return false;
    {
      unsigned int i=js["ttsChunks"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.ttsChunks.resize(i);
      while(i--)
      {
        NsSmartDeviceLinkRPCV2::TTSChunk t;
        if(!NsSmartDeviceLinkRPCV2::TTSChunkMarshaller::fromJSON(js["ttsChunks"][i],t))
          return false;
         c.ttsChunks[i]=t;
      }
    }

    if(!js.isMember("softButtons") || !js["softButtons"].isArray())  return false;
    {
      unsigned int i=js["softButtons"].size();
      if(i<0)  return false;
      if(i>3)  return false;
      c.softButtons.resize(i);
      while(i--)
      {
        NsSmartDeviceLinkRPCV2::SoftButton t;
        if(!NsSmartDeviceLinkRPCV2::SoftButtonMarshaller::fromJSON(js["softButtons"][i],t))
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
