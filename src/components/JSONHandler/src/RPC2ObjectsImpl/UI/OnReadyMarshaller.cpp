#include "../include/JSONHandler/RPC2Objects/UI/OnReady.h"
#include "OnReadyMarshaller.h"

using namespace RPC2Communication;
using namespace RPC2Communication::UI;

bool OnReadyMarshaller::checkIntegrity(OnReady& e)
{
    return true;
}
      
bool OnReadyMarshaller::checkIntegrityConst(const OnReady& e)
{
    return true;
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

Json::Value OnReadyMarshaller::toJSON(const OnReady& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnReady");
  return json;

}