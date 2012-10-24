#include "../../include/JSONHandler/ALRPCObjects/GetVehicleData_request.h"
#include "VehicleDataTypeMarshaller.h"

#include "GetVehicleData_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("GetVehicleData");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["dataType"]=VehicleDataTypeMarshaller::toJSON(e.dataType);

  json["request"]["parameters"]=j;
  return json;
}


bool GetVehicleData_requestMarshaller::fromJSON(const Json::Value& js,GetVehicleData_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("GetVehicleData"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
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

