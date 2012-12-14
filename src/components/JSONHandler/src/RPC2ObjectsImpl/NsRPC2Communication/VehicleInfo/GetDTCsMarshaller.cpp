#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetDTCs.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetDTCsMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;

bool GetDTCsMarshaller::checkIntegrity(GetDTCs& s)
{
  return checkIntegrityConst(s);
}


bool GetDTCsMarshaller::fromString(const std::string& s,GetDTCs& e)
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


const std::string GetDTCsMarshaller::toString(const GetDTCs& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetDTCsMarshaller::checkIntegrityConst(const GetDTCs& s)
{
  if(s.ecuName>65535)  return false;

  return true;
}


Json::Value GetDTCsMarshaller::toJSON(const GetDTCs& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.GetDTCs");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["ecuName"]=Json::Value(e.ecuName);;
  if(e.encrypted)
    json["params"]["encrypted"]=Json::Value(e.encrypted[0]);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool GetDTCsMarshaller::fromJSON(const Json::Value& json,GetDTCs& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.GetDTCs"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("ecuName") || !js["ecuName"].isInt())  return false;
    c.ecuName=js["ecuName"].asInt();
    if(c.ecuName>65535)  return false;

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
