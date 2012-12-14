#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/SubscribeVehicleDataResponse.h"
#include "../src/ALRPCObjectsImpl/V2/VehicleDataResultMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/SubscribeVehicleDataResponseMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;

bool SubscribeVehicleDataResponseMarshaller::checkIntegrity(SubscribeVehicleDataResponse& s)
{
  return checkIntegrityConst(s);
}


bool SubscribeVehicleDataResponseMarshaller::fromString(const std::string& s,SubscribeVehicleDataResponse& e)
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


const std::string SubscribeVehicleDataResponseMarshaller::toString(const SubscribeVehicleDataResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SubscribeVehicleDataResponseMarshaller::checkIntegrityConst(const SubscribeVehicleDataResponse& s)
{
  if(s.dataResult)
  {
    unsigned int i=s.dataResult[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value SubscribeVehicleDataResponseMarshaller::toJSON(const SubscribeVehicleDataResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPCV2::Result r(static_cast<NsAppLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("AppLinkCore.SubscribeVehicleDataResponse");

  if(e.dataResult)
  {
    unsigned int i=e.dataResult[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::VehicleDataResultMarshaller::toJSON(e.dataResult[0][i]);

    json["result"]["dataResult"]=j;
  }
  return json;
}


bool SubscribeVehicleDataResponseMarshaller::fromJSON(const Json::Value& json,SubscribeVehicleDataResponse& c)
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

    NsAppLinkRPCV2::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("AppLinkCore.SubscribeVehicleDataResponse")) return false;

    if(!NsAppLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(c.dataResult)  delete c.dataResult;
    c.dataResult=0;
    if(js.isMember("dataResult"))
    {
      if(!js["dataResult"].isArray()) return false;
      unsigned int i=js["dataResult"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.dataResult=new std::vector<NsAppLinkRPCV2::VehicleDataResult>();
      c.dataResult->resize(js["dataResult"].size());

      while(i--)
        if(!NsAppLinkRPCV2::VehicleDataResultMarshaller::fromJSON(js["dataResult"][i],c.dataResult[0][i]))  return false;
    }


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
