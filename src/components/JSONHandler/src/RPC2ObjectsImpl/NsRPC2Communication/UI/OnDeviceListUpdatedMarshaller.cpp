#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnDeviceListUpdated.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnDeviceListUpdatedMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool OnDeviceListUpdatedMarshaller::checkIntegrity(OnDeviceListUpdated& s)
{
  return checkIntegrityConst(s);
}


bool OnDeviceListUpdatedMarshaller::fromString(const std::string& s,OnDeviceListUpdated& e)
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


const std::string OnDeviceListUpdatedMarshaller::toString(const OnDeviceListUpdated& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnDeviceListUpdatedMarshaller::checkIntegrityConst(const OnDeviceListUpdated& s)
{
  {
    unsigned int i=s.deviceList.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value OnDeviceListUpdatedMarshaller::toJSON(const OnDeviceListUpdated& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnDeviceListUpdated");
  json["params"]=Json::Value(Json::objectValue);

  {
    unsigned int i=e.deviceList.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.deviceList[i]);

    json["params"]["deviceList"]=j;
  }
  return json;
}


bool OnDeviceListUpdatedMarshaller::fromJSON(const Json::Value& json,OnDeviceListUpdated& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.OnDeviceListUpdated"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

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
