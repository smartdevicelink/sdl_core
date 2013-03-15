//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/GetDeviceListResponse.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/BasicCommunication/GetDeviceListResponseMarshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;

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
  if(s.deviceList)
  {
    unsigned int i=s.deviceList[0].size();
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
  NsSmartDeviceLinkRPCV2::Result r(static_cast<NsSmartDeviceLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsSmartDeviceLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("BasicCommunication.GetDeviceListResponse");

  if(e.deviceList)
  {
    unsigned int i=e.deviceList[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.deviceList[0][i]);

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

    NsSmartDeviceLinkRPCV2::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("BasicCommunication.GetDeviceListResponse")) return false;

    if(!NsSmartDeviceLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(c.deviceList)  delete c.deviceList;
    c.deviceList=0;
    if(js.isMember("deviceList"))
    {
      if(!js["deviceList"].isArray()) return false;
      unsigned int i=js["deviceList"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.deviceList=new std::vector<std::string>();
      c.deviceList->resize(js["deviceList"].size());

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
