#include "../../include/JSONHandler/ALRPCObjects/UnsubscribeVehicleData_request.h"
#include "VehicleDataTypeMarshaller.h"

#include "UnsubscribeVehicleData_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool UnsubscribeVehicleData_requestMarshaller::checkIntegrity(UnsubscribeVehicleData_request& s)
{
  return checkIntegrityConst(s);
}


bool UnsubscribeVehicleData_requestMarshaller::fromString(const std::string& s,UnsubscribeVehicleData_request& e)
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


const std::string UnsubscribeVehicleData_requestMarshaller::toString(const UnsubscribeVehicleData_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool UnsubscribeVehicleData_requestMarshaller::checkIntegrityConst(const UnsubscribeVehicleData_request& s)
{
  if(s.dataType)
  {
    unsigned int i=s.dataType[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!VehicleDataTypeMarshaller::checkIntegrityConst(s.dataType[0][i]))   return false;
    }
  }
  return true;
}

Json::Value UnsubscribeVehicleData_requestMarshaller::toJSON(const UnsubscribeVehicleData_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("UnsubscribeVehicleData");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.dataType)
  {
    unsigned int sz=e.dataType->size();
    j["dataType"]=Json::Value(Json::arrayValue);
    j["dataType"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["dataType"][i]=VehicleDataTypeMarshaller::toJSON(e.dataType[0][i]);
  }

  json["request"]["parameters"]=j;
  return json;
}


bool UnsubscribeVehicleData_requestMarshaller::fromJSON(const Json::Value& js,UnsubscribeVehicleData_request& c)
{
  if(c.dataType)  delete c.dataType;
  c.dataType=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("UnsubscribeVehicleData"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("dataType"))
    {
      const Json::Value& j=json["dataType"];
      if(!j.isArray())  return false;
      c.dataType=new std::vector<VehicleDataType>();
      c.dataType->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        VehicleDataType t;
        if(!VehicleDataTypeMarshaller::fromJSON(j[i],t))
          return false;
        c.dataType[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

