//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/DTC.h"


#include "DTCMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool DTCMarshaller::checkIntegrity(DTC& s)
{
  return checkIntegrityConst(s);
}


bool DTCMarshaller::fromString(const std::string& s,DTC& e)
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


const std::string DTCMarshaller::toString(const DTC& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DTCMarshaller::checkIntegrityConst(const DTC& s)
{
  if(s.identifier.length()>500)  return false;
  if(s.statusByte.length()>500)  return false;
  return true;
}

Json::Value DTCMarshaller::toJSON(const DTC& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["identifier"]=Json::Value(e.identifier);

  json["statusByte"]=Json::Value(e.statusByte);


  return json;
}


bool DTCMarshaller::fromJSON(const Json::Value& json,DTC& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("identifier"))  return false;
    {
      const Json::Value& j=json["identifier"];
      if(!j.isString())  return false;
      c.identifier=j.asString();
    }
    if(!json.isMember("statusByte"))  return false;
    {
      const Json::Value& j=json["statusByte"];
      if(!j.isString())  return false;
      c.statusByte=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

