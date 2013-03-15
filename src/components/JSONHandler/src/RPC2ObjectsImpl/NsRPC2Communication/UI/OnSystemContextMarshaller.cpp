//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnSystemContext.h"
#include "../src/SDLRPCObjectsImpl/V2/SystemContextMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnSystemContextMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;

bool OnSystemContextMarshaller::checkIntegrity(OnSystemContext& s)
{
  return checkIntegrityConst(s);
}


bool OnSystemContextMarshaller::fromString(const std::string& s,OnSystemContext& e)
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


const std::string OnSystemContextMarshaller::toString(const OnSystemContext& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnSystemContextMarshaller::checkIntegrityConst(const OnSystemContext& s)
{
  if(!NsSmartDeviceLinkRPCV2::SystemContextMarshaller::checkIntegrityConst(s.systemContext))  return false;

  return true;
}


Json::Value OnSystemContextMarshaller::toJSON(const OnSystemContext& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnSystemContext");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["systemContext"]=NsSmartDeviceLinkRPCV2::SystemContextMarshaller::toJSON(e.systemContext);;
  return json;
}


bool OnSystemContextMarshaller::fromJSON(const Json::Value& json,OnSystemContext& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.OnSystemContext"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("systemContext") || !NsSmartDeviceLinkRPCV2::SystemContextMarshaller::fromJSON(js["systemContext"],c.systemContext))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
