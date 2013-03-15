//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/GetCapabilitiesResponse.h"
#include "../src/SDLRPCObjectsImpl/V2/ButtonCapabilitiesMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/PresetBankCapabilitiesMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/GetCapabilitiesResponseMarshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::Buttons;

bool GetCapabilitiesResponseMarshaller::checkIntegrity(GetCapabilitiesResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetCapabilitiesResponseMarshaller::fromString(const std::string& s,GetCapabilitiesResponse& e)
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


const std::string GetCapabilitiesResponseMarshaller::toString(const GetCapabilitiesResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetCapabilitiesResponseMarshaller::checkIntegrityConst(const GetCapabilitiesResponse& s)
{
  {
    unsigned int i=s.capabilities.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  if(s.presetBankCapabilities && (!NsSmartDeviceLinkRPCV2::PresetBankCapabilitiesMarshaller::checkIntegrityConst(s.presetBankCapabilities[0])))  return false;

  return true;
}


Json::Value GetCapabilitiesResponseMarshaller::toJSON(const GetCapabilitiesResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsSmartDeviceLinkRPCV2::Result r(static_cast<NsSmartDeviceLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsSmartDeviceLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("Buttons.GetCapabilitiesResponse");

  {
    unsigned int i=e.capabilities.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::ButtonCapabilitiesMarshaller::toJSON(e.capabilities[i]);

    json["result"]["capabilities"]=j;
  }
  if(e.presetBankCapabilities)
    json["result"]["presetBankCapabilities"]=NsSmartDeviceLinkRPCV2::PresetBankCapabilitiesMarshaller::toJSON(e.presetBankCapabilities[0]);;
  return json;
}


bool GetCapabilitiesResponseMarshaller::fromJSON(const Json::Value& json,GetCapabilitiesResponse& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("result")) return false;

    Json::Value js=json["result"];
    if(!js.isObject())  return false;

    NsSmartDeviceLinkRPCV2::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("Buttons.GetCapabilitiesResponse")) return false;

    if(!NsSmartDeviceLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("capabilities") || !js["capabilities"].isArray())  return false;
    {
      unsigned int i=js["capabilities"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.capabilities.resize(i);
      while(i--)
      {
        NsSmartDeviceLinkRPCV2::ButtonCapabilities t;
        if(!NsSmartDeviceLinkRPCV2::ButtonCapabilitiesMarshaller::fromJSON(js["capabilities"][i],t))
          return false;
         c.capabilities[i]=t;
      }
    }

    if(c.presetBankCapabilities)  delete c.presetBankCapabilities;
    c.presetBankCapabilities=0;
    if(js.isMember("presetBankCapabilities"))
    {
      c.presetBankCapabilities=new NsSmartDeviceLinkRPCV2::PresetBankCapabilities();
      if(!NsSmartDeviceLinkRPCV2::PresetBankCapabilitiesMarshaller::fromJSON(js["presetBankCapabilities"],c.presetBankCapabilities[0]))  return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
