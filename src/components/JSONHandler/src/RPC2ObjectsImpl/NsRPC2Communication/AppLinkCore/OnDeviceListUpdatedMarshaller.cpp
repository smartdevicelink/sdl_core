#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnDeviceListUpdated.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnDeviceListUpdatedMarshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;

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
  if (s.deviceList)
  {
    unsigned int i=s.deviceList->size();
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
  json["method"]=Json::Value("AppLinkCore.OnDeviceListUpdated");
  json["params"]=Json::Value(Json::objectValue);

  if (e.deviceList)
  {
    unsigned int i=e.deviceList->size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.deviceList[0][i]);

    json["params"]["deviceList"]=j;
  }
  return json;
}


bool OnDeviceListUpdatedMarshaller::fromJSON(const Json::Value& json,OnDeviceListUpdated& c)
{
  delete c.deviceList;
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.OnDeviceListUpdated"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(js.isMember("deviceList")) 
    {
      if (!js["deviceList"].isArray()) return false;
      unsigned int i=js["deviceList"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.deviceList = new std::vector<std::string> (i);
      while(i--)
      {
        if(!js["deviceList"][i].isString())
          return false;
        c.deviceList[0][i]=js["deviceList"][i].asString();
        
      }
    }


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
