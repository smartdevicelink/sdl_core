#include "../include/JSONHandler/ALRPCObjects/SubscribeVehicleData_request.h"
#include "VehicleDataTypeMarshaller.h"

#include "SubscribeVehicleData_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


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

Json::Value SubscribeVehicleData_requestMarshaller::toJSON(const SubscribeVehicleData_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("SubscribeVehicleData");
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


bool SubscribeVehicleData_requestMarshaller::fromJSON(const Json::Value& js,SubscribeVehicleData_request& c)
{
  if(c.dataType)  delete c.dataType;
  c.dataType=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("SubscribeVehicleData"))  return false;
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

