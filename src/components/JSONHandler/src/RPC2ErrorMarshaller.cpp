/**
* \file RPC2ErrorMarshaller.cpp
* \brief RPC2ErrorMarshaller class source file.
* Copyright (c) 2013 Ford Motor Company
*/


#include <cstdlib>
#include <cstring>
#include "JSONHandler/RPC2ErrorMarshaller.h"

using namespace NsRPC2Communication;


bool RPC2ErrorMarshaller::fromString(const std::string& s,RPC2Error& e)
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


const std::string RPC2ErrorMarshaller::toString(const RPC2Error& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value RPC2ErrorMarshaller::toJSON(const RPC2Error& e)
{
  Json::Value json(Json::objectValue);

  json["jsonrpc"]=Json::Value("2.0");

  json["id"]= e.mId ? Json::Value(e.mId) : Json::Value(Json::nullValue);

  json["error"]=Json::Value(Json::objectValue);

  json["error"]["code"]=Json::Value(e.mCode);
  json["error"]["message"]=Json::Value(e.mMessage);

  return json;
}


bool RPC2ErrorMarshaller::fromJSON(const Json::Value& json,RPC2Error& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("error")) return false;
    Json::Value js=json["error"];

    if(json.isMember("id") && !json["id"].isNull())
    {
      if(!json["id"].isInt())
        return false;
      c.mId=json["id"].asInt();
    }
    else
      c.mId=0;

    if(!js.isObject())  return false;
    if(!js.isMember("code"))  return false;
    if(!js.isMember("message"))  return false;
    if(!js["code"].isInt())  return false;
    if(!js["message"].isString())  return false;
    
    c.mCode=js["code"].asInt();
    c.mMessage=js["message"].asString();
  }
  catch(...)
  {
    return false;
  }
  return true;
}

