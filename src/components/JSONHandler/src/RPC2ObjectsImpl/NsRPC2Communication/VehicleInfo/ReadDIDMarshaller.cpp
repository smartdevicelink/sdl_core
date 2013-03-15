//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/ReadDID.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/ReadDIDMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;

bool ReadDIDMarshaller::checkIntegrity(ReadDID& s)
{
  return checkIntegrityConst(s);
}


bool ReadDIDMarshaller::fromString(const std::string& s,ReadDID& e)
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


const std::string ReadDIDMarshaller::toString(const ReadDID& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ReadDIDMarshaller::checkIntegrityConst(const ReadDID& s)
{
  if(s.ecuName>65535)  return false;

  {
    unsigned int i=s.didLocation.size();
    if(i<0)  return false;
    if(i>1000)  return false;
    while(i--)
    {
      if(s.didLocation[i]>65535)  return false;
    }
  }

  return true;
}


Json::Value ReadDIDMarshaller::toJSON(const ReadDID& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("VehicleInfo.ReadDID");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["ecuName"]=Json::Value(e.ecuName);;
  {
    unsigned int i=e.didLocation.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.didLocation[i]);

    json["params"]["didLocation"]=j;
  }
  if(e.encrypted)
    json["params"]["encrypted"]=Json::Value(e.encrypted[0]);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool ReadDIDMarshaller::fromJSON(const Json::Value& json,ReadDID& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("VehicleInfo.ReadDID"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("ecuName") || !js["ecuName"].isInt())  return false;
    c.ecuName=js["ecuName"].asInt();
    if(c.ecuName>65535)  return false;

    if(!js.isMember("didLocation") || !js["didLocation"].isArray())
      return false;
    {
      c.didLocation.clear();
      unsigned int i=js["didLocation"].size();
      if(i<0)  return false;
      if(i>1000)  return false;
      c.didLocation.resize(i);
      while(i--)
      {
        if(!js["didLocation"][i].isInt())
          return false;
        c.didLocation[i]=js["didLocation"][i].asInt();
        if(c.didLocation[i]>65535)  return false;

      }
    }


    if(c.encrypted)  delete c.encrypted;
    c.encrypted=0;
    if(js.isMember("encrypted"))
    {
      if(!js["encrypted"].isBool())  return false;
      c.encrypted=new bool();
      c.encrypted[0]=js["encrypted"].asBool();
      
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
