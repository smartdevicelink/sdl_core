//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DialNumber.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DialNumberMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;

bool DialNumberMarshaller::checkIntegrity(DialNumber& s)
{
  return checkIntegrityConst(s);
}


bool DialNumberMarshaller::fromString(const std::string& s,DialNumber& e)
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


const std::string DialNumberMarshaller::toString(const DialNumber& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DialNumberMarshaller::checkIntegrityConst(const DialNumber& s)
{
  if(s.number.length()>500)  return false;

  return true;
}


Json::Value DialNumberMarshaller::toJSON(const DialNumber& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.DialNumber");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["number"]=Json::Value(e.number);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool DialNumberMarshaller::fromJSON(const Json::Value& json,DialNumber& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.DialNumber"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("number") || !js["number"].isString())  return false;
    c.number=js["number"].asString();
    if(c.number.length()>500)  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
