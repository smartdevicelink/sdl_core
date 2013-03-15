//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SubscribeVehicleData_request.h"
#include "VehicleDataTypeMarshaller.h"

#include "SubscribeVehicleData_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool SubscribeVehicleData_requestMarshaller::checkIntegrity(SubscribeVehicleData_request& s)
{
  return checkIntegrityConst(s);
}


bool SubscribeVehicleData_requestMarshaller::fromString(const std::string& s,SubscribeVehicleData_request& e)
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


const std::string SubscribeVehicleData_requestMarshaller::toString(const SubscribeVehicleData_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SubscribeVehicleData_requestMarshaller::checkIntegrityConst(const SubscribeVehicleData_request& s)
{
  {
    unsigned int i=s.dataType.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!VehicleDataTypeMarshaller::checkIntegrityConst(s.dataType[i]))   return false;
    }
  }
  return true;
}

Json::Value SubscribeVehicleData_requestMarshaller::toJSON(const SubscribeVehicleData_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["dataType"]=Json::Value(Json::arrayValue);
  json["dataType"].resize(e.dataType.size());
  for(unsigned int i=0;i<e.dataType.size();i++)
    json["dataType"][i]=VehicleDataTypeMarshaller::toJSON(e.dataType[i]);

  return json;
}


bool SubscribeVehicleData_requestMarshaller::fromJSON(const Json::Value& json,SubscribeVehicleData_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("dataType"))  return false;
    {
      const Json::Value& j=json["dataType"];
      if(!j.isArray())  return false;
      c.dataType.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          VehicleDataType t;
          if(!VehicleDataTypeMarshaller::fromJSON(j[i],t))
            return false;
          c.dataType[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

