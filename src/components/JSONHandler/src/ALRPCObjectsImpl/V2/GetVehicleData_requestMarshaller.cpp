#include "../include/JSONHandler/ALRPCObjects/V2/GetVehicleData_request.h"
#include "VehicleDataTypeMarshaller.h"

#include "GetVehicleData_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool GetVehicleData_requestMarshaller::checkIntegrity(GetVehicleData_request& s)
{
  return checkIntegrityConst(s);
}


bool GetVehicleData_requestMarshaller::fromString(const std::string& s,GetVehicleData_request& e)
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


const std::string GetVehicleData_requestMarshaller::toString(const GetVehicleData_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetVehicleData_requestMarshaller::checkIntegrityConst(const GetVehicleData_request& s)
{
  if(!VehicleDataTypeMarshaller::checkIntegrityConst(s.dataType))  return false;
  return true;
}

Json::Value GetVehicleData_requestMarshaller::toJSON(const GetVehicleData_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["dataType"]=VehicleDataTypeMarshaller::toJSON(e.dataType);

  return json;
}


bool GetVehicleData_requestMarshaller::fromJSON(const Json::Value& json,GetVehicleData_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("dataType"))  return false;
    {
      const Json::Value& j=json["dataType"];
      if(!VehicleDataTypeMarshaller::fromJSON(j,c.dataType))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

