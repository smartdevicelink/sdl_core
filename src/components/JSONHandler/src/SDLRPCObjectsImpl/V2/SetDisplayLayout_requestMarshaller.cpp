//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SetDisplayLayout_request.h"


#include "SetDisplayLayout_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool SetDisplayLayout_requestMarshaller::checkIntegrity(SetDisplayLayout_request& s)
{
  return checkIntegrityConst(s);
}


bool SetDisplayLayout_requestMarshaller::fromString(const std::string& s,SetDisplayLayout_request& e)
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


const std::string SetDisplayLayout_requestMarshaller::toString(const SetDisplayLayout_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetDisplayLayout_requestMarshaller::checkIntegrityConst(const SetDisplayLayout_request& s)
{
  return true;
}

Json::Value SetDisplayLayout_requestMarshaller::toJSON(const SetDisplayLayout_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["displayLayout"]=Json::Value(e.displayLayout);

  return json;
}


bool SetDisplayLayout_requestMarshaller::fromJSON(const Json::Value& json,SetDisplayLayout_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("displayLayout"))  return false;
    {
      const Json::Value& j=json["displayLayout"];
      if(!j.isString())  return false;
      c.displayLayout=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

