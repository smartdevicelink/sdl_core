#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnReady.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnReadyMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool OnReadyMarshaller::checkIntegrity(OnReady& s)
{
  return checkIntegrityConst(s);
}


bool OnReadyMarshaller::fromString(const std::string& s,OnReady& e)
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


const std::string OnReadyMarshaller::toString(const OnReady& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnReadyMarshaller::checkIntegrityConst(const OnReady& s)
{
  return true;
}


Json::Value OnReadyMarshaller::toJSON(const OnReady& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnReady");

  return json;
}


bool OnReadyMarshaller::fromJSON(const Json::Value& json,OnReady& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.OnReady"))  return false;
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
