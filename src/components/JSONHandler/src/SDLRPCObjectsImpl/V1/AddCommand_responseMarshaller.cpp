//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/AddCommand_response.h"
#include "ResultMarshaller.h"

#include "AddCommand_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;


bool AddCommand_responseMarshaller::checkIntegrity(AddCommand_response& s)
{
  return checkIntegrityConst(s);
}


bool AddCommand_responseMarshaller::fromString(const std::string& s,AddCommand_response& e)
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


const std::string AddCommand_responseMarshaller::toString(const AddCommand_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AddCommand_responseMarshaller::checkIntegrityConst(const AddCommand_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(static_cast<NsSmartDeviceLinkRPCV2::Result>(s.resultCode)))  return false;
  if(s.info && s.info->length()>1000)  return false;
  return true;
}

Json::Value AddCommand_responseMarshaller::toJSON(const AddCommand_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["response"]=Json::Value(Json::objectValue);
  json["response"]["name"]=Json::Value("AddCommand");
  json["response"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["success"]=Json::Value(e.success);

  j["resultCode"]=ResultMarshaller::toJSON(static_cast<NsSmartDeviceLinkRPCV2::Result>(e.resultCode));

  if(e.info)
    j["info"]=Json::Value(*e.info);

  json["response"]["parameters"]=j;
  return json;
}


bool AddCommand_responseMarshaller::fromJSON(const Json::Value& js,AddCommand_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("response"))  return false;

    if(!js["response"].isObject())  return false;
    const Json::Value& j2=js["response"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("AddCommand"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("success"))  return false;
    {
      const Json::Value& j=json["success"];
      if(!j.isBool())  return false;
      c.success=j.asBool();
    }
    if(!json.isMember("resultCode"))  return false;
    {
      const Json::Value& j=json["resultCode"];
      NsSmartDeviceLinkRPC::Result result = static_cast<NsSmartDeviceLinkRPC::Result>(c.resultCode);
      if(!ResultMarshaller::fromJSON(j, result))
        return false;
      c.resultCode = static_cast<NsSmartDeviceLinkRPCV2::Result>(result);
    }
    if(json.isMember("info"))
    {
      const Json::Value& j=json["info"];
      if(!j.isString())  return false;
      c.info=new std::string(j.asString());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

