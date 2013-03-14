#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleTypeResponse.h"
#include "../src/SDLRPCObjectsImpl/V2/VehicleTypeMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetVehicleTypeResponseMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;

bool GetVehicleTypeResponseMarshaller::checkIntegrity(GetVehicleTypeResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetVehicleTypeResponseMarshaller::fromString(const std::string& s,GetVehicleTypeResponse& e)
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


const std::string GetVehicleTypeResponseMarshaller::toString(const GetVehicleTypeResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetVehicleTypeResponseMarshaller::checkIntegrityConst(const GetVehicleTypeResponse& s)
{
  if(!NsSmartDeviceLinkRPCV2::VehicleTypeMarshaller::checkIntegrityConst(s.vehicleType))  return false;

  return true;
}


Json::Value GetVehicleTypeResponseMarshaller::toJSON(const GetVehicleTypeResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsSmartDeviceLinkRPCV2::Result r(static_cast<NsSmartDeviceLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsSmartDeviceLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("VehicleInfo.GetVehicleTypeResponse");

  json["result"]["vehicleType"]=NsSmartDeviceLinkRPCV2::VehicleTypeMarshaller::toJSON(e.vehicleType);;
  return json;
}


bool GetVehicleTypeResponseMarshaller::fromJSON(const Json::Value& json,GetVehicleTypeResponse& c)
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
    if(js["method"].asString().compare("VehicleInfo.GetVehicleTypeResponse")) return false;

    if(!NsSmartDeviceLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("vehicleType") || !NsSmartDeviceLinkRPCV2::VehicleTypeMarshaller::fromJSON(js["vehicleType"],c.vehicleType))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
