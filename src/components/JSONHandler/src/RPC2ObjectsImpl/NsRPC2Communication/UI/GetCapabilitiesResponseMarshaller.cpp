#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/ALRPCObjectsImpl/DisplayCapabilitiesMarshaller.h"
#include "../src/ALRPCObjectsImpl/HmiZoneCapabilitiesMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetCapabilitiesResponseMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

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
  if(!NsAppLinkRPC::DisplayCapabilitiesMarshaller::checkIntegrityConst(s.displayCapabilities))  return false;

  {
    unsigned int i=s.hmiZoneCapabilities.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

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
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.GetCapabilitiesResponse");

  json["result"]["displayCapabilities"]=NsAppLinkRPC::DisplayCapabilitiesMarshaller::toJSON(e.displayCapabilities);;
  {
    unsigned int i=e.hmiZoneCapabilities.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPC::HmiZoneCapabilitiesMarshaller::toJSON(e.hmiZoneCapabilities[i]);

    json["result"]["hmiZoneCapabilities"]=j;
  }
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

    NsAppLinkRPC::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("UI.GetCapabilitiesResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("displayCapabilities") || !NsAppLinkRPC::DisplayCapabilitiesMarshaller::fromJSON(js["displayCapabilities"],c.displayCapabilities))  return false;

    if(!js.isMember("hmiZoneCapabilities") || !js["hmiZoneCapabilities"].isArray())  return false;
    {
      unsigned int i=js["hmiZoneCapabilities"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      std::vector<NsAppLinkRPC::HmiZoneCapabilities> z(i);
      while(i--)
        if(!NsAppLinkRPC::HmiZoneCapabilitiesMarshaller::fromJSON(js["hmiZoneCapabilities"][i],c.hmiZoneCapabilities[i]))  return false;
      c.hmiZoneCapabilities=z;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
