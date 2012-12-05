#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertResponse.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AlertResponseMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool AlertResponseMarshaller::checkIntegrity(AlertResponse& s)
{
  return checkIntegrityConst(s);
}


bool AlertResponseMarshaller::fromString(const std::string& s,AlertResponse& e)
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


const std::string AlertResponseMarshaller::toString(const AlertResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AlertResponseMarshaller::checkIntegrityConst(const AlertResponse& s)
{
  if(s.tryAgainTime && (s.tryAgainTime[0]>2000000000))  return false;

  return true;
}


Json::Value AlertResponseMarshaller::toJSON(const AlertResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.AlertResponse");

  if(e.tryAgainTime)
    json["result"]["tryAgainTime"]=Json::Value(e.tryAgainTime[0]);;
  return json;
}


bool AlertResponseMarshaller::fromJSON(const Json::Value& json,AlertResponse& c)
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
    if(js["method"].asString().compare("UI.AlertResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(c.tryAgainTime)  delete c.tryAgainTime;
    c.tryAgainTime=0;
    if(js.isMember("tryAgainTime"))
    {
      if(!js["tryAgainTime"].isInt())  return false;
      c.tryAgainTime=new unsigned int();
      c.tryAgainTime[0]=js["tryAgainTime"].asInt();
      if(c.tryAgainTime[0]>2000000000)  return false;

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
