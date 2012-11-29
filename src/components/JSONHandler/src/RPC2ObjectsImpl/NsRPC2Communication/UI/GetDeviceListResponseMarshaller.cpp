#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetDeviceListResponse.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetDeviceListResponseMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool GetDeviceListResponseMarshaller::checkIntegrity(GetDeviceListResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetDeviceListResponseMarshaller::fromString(const std::string& s,GetDeviceListResponse& e)
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


const std::string GetDeviceListResponseMarshaller::toString(const GetDeviceListResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetDeviceListResponseMarshaller::checkIntegrityConst(const GetDeviceListResponse& s)
{
  {
    unsigned int i=s.deviceList.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value GetDeviceListResponseMarshaller::toJSON(const GetDeviceListResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.GetDeviceListResponse");

  {
    unsigned int i=e.deviceList.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.deviceList[i]);

    json["result"]["deviceList"]=j;
  }
  return json;
}


bool GetDeviceListResponseMarshaller::fromJSON(const Json::Value& json,GetDeviceListResponse& c)
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
    if(js["method"].asString().compare("UI.GetDeviceListResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("deviceList") || !js["deviceList"].isArray())
      return false;
    {
      c.deviceList.clear();
      unsigned int i=js["deviceList"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.deviceList.resize(i);
      while(i--)
      {
        if(!js["deviceList"][i].isString())
          return false;
        c.deviceList[i]=js["deviceList"][i].asString();
        
      }
    }


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
