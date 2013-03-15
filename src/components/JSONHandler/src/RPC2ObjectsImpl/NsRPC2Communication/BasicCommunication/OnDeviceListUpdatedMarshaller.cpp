//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/OnDeviceListUpdated.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/RPC2ObjectsImpl//NsRPC2Communication/BasicCommunication/OnDeviceListUpdatedMarshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;

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
  if(s.deviceList)
  {
    unsigned int i=s.deviceList[0].size();
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
  json["method"]=Json::Value("BasicCommunication.OnDeviceListUpdated");
  json["params"]=Json::Value(Json::objectValue);

  if(e.deviceList)
  {
    unsigned int i=e.deviceList[0].size();
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
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("BasicCommunication.OnDeviceListUpdated"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

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
