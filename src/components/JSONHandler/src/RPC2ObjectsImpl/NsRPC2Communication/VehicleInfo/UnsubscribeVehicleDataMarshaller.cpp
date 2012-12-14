#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/UnsubscribeVehicleData.h"
#include "../src/ALRPCObjectsImpl/V2/VehicleDataTypeMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/UnsubscribeVehicleDataMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;

bool UnsubscribeVehicleDataMarshaller::checkIntegrity(UnsubscribeVehicleData& s)
{
  return checkIntegrityConst(s);
}


bool UnsubscribeVehicleDataMarshaller::fromString(const std::string& s,UnsubscribeVehicleData& e)
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


const std::string UnsubscribeVehicleDataMarshaller::toString(const UnsubscribeVehicleData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UnsubscribeVehicleDataMarshaller::checkIntegrityConst(const UnsubscribeVehicleData& s)
{
  {
    unsigned int i=s.dataType.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value UnsubscribeVehicleDataMarshaller::toJSON(const UnsubscribeVehicleData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.UnsubscribeVehicleData");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  {
    unsigned int i=e.dataType.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::VehicleDataTypeMarshaller::toJSON(e.dataType[i]);

    json["params"]["dataType"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool UnsubscribeVehicleDataMarshaller::fromJSON(const Json::Value& json,UnsubscribeVehicleData& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.UnsubscribeVehicleData"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("dataType") || !js["dataType"].isArray())  return false;
    {
      unsigned int i=js["dataType"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.dataType.resize(i);
      while(i--)
      {
        NsAppLinkRPCV2::VehicleDataType t;
        if(!NsAppLinkRPCV2::VehicleDataTypeMarshaller::fromJSON(js["dataType"][i],t))
          return false;
         c.dataType[i]=t;
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
