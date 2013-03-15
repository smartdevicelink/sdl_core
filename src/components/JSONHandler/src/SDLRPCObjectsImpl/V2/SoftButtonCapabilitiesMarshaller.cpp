//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SoftButtonCapabilities.h"


#include "SoftButtonCapabilitiesMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool SoftButtonCapabilitiesMarshaller::checkIntegrity(SoftButtonCapabilities& s)
{
  return checkIntegrityConst(s);
}


bool SoftButtonCapabilitiesMarshaller::fromString(const std::string& s,SoftButtonCapabilities& e)
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


const std::string SoftButtonCapabilitiesMarshaller::toString(const SoftButtonCapabilities& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SoftButtonCapabilitiesMarshaller::checkIntegrityConst(const SoftButtonCapabilities& s)
{
  return true;
}

Json::Value SoftButtonCapabilitiesMarshaller::toJSON(const SoftButtonCapabilities& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["imageSupported"]=Json::Value(e.imageSupported);

  json["longPressAvailable"]=Json::Value(e.longPressAvailable);

  json["shortPressAvailable"]=Json::Value(e.shortPressAvailable);

  json["upDownAvailable"]=Json::Value(e.upDownAvailable);


  return json;
}


bool SoftButtonCapabilitiesMarshaller::fromJSON(const Json::Value& json,SoftButtonCapabilities& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("imageSupported"))  return false;
    {
      const Json::Value& j=json["imageSupported"];
      if(!j.isBool())  return false;
      c.imageSupported=j.asBool();
    }
    if(!json.isMember("longPressAvailable"))  return false;
    {
      const Json::Value& j=json["longPressAvailable"];
      if(!j.isBool())  return false;
      c.longPressAvailable=j.asBool();
    }
    if(!json.isMember("shortPressAvailable"))  return false;
    {
      const Json::Value& j=json["shortPressAvailable"];
      if(!j.isBool())  return false;
      c.shortPressAvailable=j.asBool();
    }
    if(!json.isMember("upDownAvailable"))  return false;
    {
      const Json::Value& j=json["upDownAvailable"];
      if(!j.isBool())  return false;
      c.upDownAvailable=j.asBool();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

