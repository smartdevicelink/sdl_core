//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SetAppIcon_request.h"


#include "SetAppIcon_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool SetAppIcon_requestMarshaller::checkIntegrity(SetAppIcon_request& s)
{
  return checkIntegrityConst(s);
}


bool SetAppIcon_requestMarshaller::fromString(const std::string& s,SetAppIcon_request& e)
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


const std::string SetAppIcon_requestMarshaller::toString(const SetAppIcon_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetAppIcon_requestMarshaller::checkIntegrityConst(const SetAppIcon_request& s)
{
  if(s.syncFileName.length()>500)  return false;
  return true;
}

Json::Value SetAppIcon_requestMarshaller::toJSON(const SetAppIcon_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["syncFileName"]=Json::Value(e.syncFileName);

  return json;
}


bool SetAppIcon_requestMarshaller::fromJSON(const Json::Value& json,SetAppIcon_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("syncFileName"))  return false;
    {
      const Json::Value& j=json["syncFileName"];
      if(!j.isString())  return false;
      c.syncFileName=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

