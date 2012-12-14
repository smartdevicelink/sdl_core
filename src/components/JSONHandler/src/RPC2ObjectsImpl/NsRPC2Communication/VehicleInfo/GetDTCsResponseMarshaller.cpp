#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetDTCsResponse.h"
#include "../src/ALRPCObjectsImpl/V1/DTCMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetDTCsResponseMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;

bool GetDTCsResponseMarshaller::checkIntegrity(GetDTCsResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetDTCsResponseMarshaller::fromString(const std::string& s,GetDTCsResponse& e)
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


const std::string GetDTCsResponseMarshaller::toString(const GetDTCsResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetDTCsResponseMarshaller::checkIntegrityConst(const GetDTCsResponse& s)
{
  if(s.dtcList)
  {
    unsigned int i=s.dtcList[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value GetDTCsResponseMarshaller::toJSON(const GetDTCsResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("AppLinkCore.GetDTCsResponse");

  if(e.dtcList)
  {
    unsigned int i=e.dtcList[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPC::DTCMarshaller::toJSON(e.dtcList[0][i]);

    json["result"]["dtcList"]=j;
  }
  return json;
}


bool GetDTCsResponseMarshaller::fromJSON(const Json::Value& json,GetDTCsResponse& c)
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
    if(js["method"].asString().compare("AppLinkCore.GetDTCsResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(c.dtcList)  delete c.dtcList;
    c.dtcList=0;
    if(js.isMember("dtcList"))
    {
      if(!js["dtcList"].isArray()) return false;
      unsigned int i=js["dtcList"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.dtcList=new std::vector<NsAppLinkRPC::DTC>();
      c.dtcList->resize(js["dtcList"].size());

      while(i--)
        if(!NsAppLinkRPC::DTCMarshaller::fromJSON(js["dtcList"][i],c.dtcList[0][i]))  return false;
    }


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
