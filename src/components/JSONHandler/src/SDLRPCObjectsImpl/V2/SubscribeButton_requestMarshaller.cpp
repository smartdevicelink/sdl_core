//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SubscribeButton_request.h"
#include "ButtonNameMarshaller.h"

#include "SubscribeButton_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool SubscribeButton_requestMarshaller::checkIntegrity(SubscribeButton_request& s)
{
  return checkIntegrityConst(s);
}


bool SubscribeButton_requestMarshaller::fromString(const std::string& s,SubscribeButton_request& e)
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


const std::string SubscribeButton_requestMarshaller::toString(const SubscribeButton_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SubscribeButton_requestMarshaller::checkIntegrityConst(const SubscribeButton_request& s)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(s.buttonName))  return false;
  return true;
}

Json::Value SubscribeButton_requestMarshaller::toJSON(const SubscribeButton_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["buttonName"]=ButtonNameMarshaller::toJSON(e.buttonName);

  return json;
}


bool SubscribeButton_requestMarshaller::fromJSON(const Json::Value& json,SubscribeButton_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("buttonName"))  return false;
    {
      const Json::Value& j=json["buttonName"];
      if(!ButtonNameMarshaller::fromJSON(j,c.buttonName))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

