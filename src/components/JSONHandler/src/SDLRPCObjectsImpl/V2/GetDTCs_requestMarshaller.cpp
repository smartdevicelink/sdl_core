//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/GetDTCs_request.h"


#include "GetDTCs_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool GetDTCs_requestMarshaller::checkIntegrity(GetDTCs_request& s)
{
  return checkIntegrityConst(s);
}


bool GetDTCs_requestMarshaller::fromString(const std::string& s,GetDTCs_request& e)
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


const std::string GetDTCs_requestMarshaller::toString(const GetDTCs_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetDTCs_requestMarshaller::checkIntegrityConst(const GetDTCs_request& s)
{
  if(s.ecuName>65535)  return false;
  return true;
}

Json::Value GetDTCs_requestMarshaller::toJSON(const GetDTCs_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["ecuName"]=Json::Value(e.ecuName);

  if(e.encrypted)
    json["encrypted"]=Json::Value(*e.encrypted);

  return json;
}


bool GetDTCs_requestMarshaller::fromJSON(const Json::Value& json,GetDTCs_request& c)
{
  if(c.encrypted)  delete c.encrypted;
  c.encrypted=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("ecuName"))  return false;
    {
      const Json::Value& j=json["ecuName"];
      if(!j.isInt())  return false;
      c.ecuName=j.asInt();
    }
    if(json.isMember("encrypted"))
    {
      const Json::Value& j=json["encrypted"];
      if(!j.isBool())  return false;
      c.encrypted=new bool(j.asBool());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

