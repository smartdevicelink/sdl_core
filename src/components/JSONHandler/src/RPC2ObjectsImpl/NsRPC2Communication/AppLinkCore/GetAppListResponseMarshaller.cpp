#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetAppListResponse.h"
#include "../src/ALRPCObjectsImpl/V2/HMIApplicationMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/GetAppListResponseMarshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;

bool GetAppListResponseMarshaller::checkIntegrity(GetAppListResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetAppListResponseMarshaller::fromString(const std::string& s,GetAppListResponse& e)
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


const std::string GetAppListResponseMarshaller::toString(const GetAppListResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetAppListResponseMarshaller::checkIntegrityConst(const GetAppListResponse& s)
{
  {
    unsigned int i=s.appList.size();
    if(i<0)  return false;
  }

  return true;
}


Json::Value GetAppListResponseMarshaller::toJSON(const GetAppListResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPCV2::Result r(static_cast<NsAppLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("AppLinkCore.GetAppListResponse");

  {
    unsigned int i=e.appList.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::HMIApplicationMarshaller::toJSON(e.appList[i]);

    json["result"]["appList"]=j;
  }
  return json;
}


bool GetAppListResponseMarshaller::fromJSON(const Json::Value& json,GetAppListResponse& c)
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
    if(js["method"].asString().compare("AppLinkCore.GetAppListResponse")) return false;

    if(!NsAppLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("appList") || !js["appList"].isArray())  return false;
    {
      unsigned int i=js["appList"].size();
      if(i<0)  return false;
      c.appList.resize(i);
      while(i--)
      {
        NsAppLinkRPCV2::HMIApplication t;
        if(!NsAppLinkRPCV2::HMIApplicationMarshaller::fromJSON(js["appList"][i],t))
          return false;
         c.appList[i]=t;
      }
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
