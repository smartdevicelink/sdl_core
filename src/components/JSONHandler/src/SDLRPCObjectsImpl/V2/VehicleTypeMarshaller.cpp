//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/VehicleType.h"


#include "VehicleTypeMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool VehicleTypeMarshaller::checkIntegrity(VehicleType& s)
{
  return checkIntegrityConst(s);
}


bool VehicleTypeMarshaller::fromString(const std::string& s,VehicleType& e)
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


const std::string VehicleTypeMarshaller::toString(const VehicleType& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool VehicleTypeMarshaller::checkIntegrityConst(const VehicleType& s)
{
  if(s.make && s.make->length()>500)  return false;
  if(s.model && s.model->length()>500)  return false;
  if(s.modelYear && s.modelYear->length()>500)  return false;
  if(s.trim && s.trim->length()>500)  return false;
  return true;
}

Json::Value VehicleTypeMarshaller::toJSON(const VehicleType& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  if(e.make)
    json["make"]=Json::Value(*e.make);

  if(e.model)
    json["model"]=Json::Value(*e.model);

  if(e.modelYear)
    json["modelYear"]=Json::Value(*e.modelYear);

  if(e.trim)
    json["trim"]=Json::Value(*e.trim);


  return json;
}


bool VehicleTypeMarshaller::fromJSON(const Json::Value& json,VehicleType& c)
{
  if(c.make)  delete c.make;
  c.make=0;

  if(c.model)  delete c.model;
  c.model=0;

  if(c.modelYear)  delete c.modelYear;
  c.modelYear=0;

  if(c.trim)  delete c.trim;
  c.trim=0;

  try
  {
    if(!json.isObject())  return false;

    if(json.isMember("make"))
    {
      const Json::Value& j=json["make"];
      if(!j.isString())  return false;
      c.make=new std::string(j.asString());
    }
    if(json.isMember("model"))
    {
      const Json::Value& j=json["model"];
      if(!j.isString())  return false;
      c.model=new std::string(j.asString());
    }
    if(json.isMember("modelYear"))
    {
      const Json::Value& j=json["modelYear"];
      if(!j.isString())  return false;
      c.modelYear=new std::string(j.asString());
    }
    if(json.isMember("trim"))
    {
      const Json::Value& j=json["trim"];
      if(!j.isString())  return false;
      c.trim=new std::string(j.asString());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

