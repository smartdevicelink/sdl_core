#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeVehicleData_response.h"
#include "ResultMarshaller.h"
#include "VehicleDataResultMarshaller.h"

#include "SubscribeVehicleData_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool SubscribeVehicleData_responseMarshaller::checkIntegrity(SubscribeVehicleData_response& s)
{
  return checkIntegrityConst(s);
}


bool SubscribeVehicleData_responseMarshaller::fromString(const std::string& s,SubscribeVehicleData_response& e)
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


const std::string SubscribeVehicleData_responseMarshaller::toString(const SubscribeVehicleData_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SubscribeVehicleData_responseMarshaller::checkIntegrityConst(const SubscribeVehicleData_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.info && s.info->length()>1000)  return false;
  if(s.dataResult)
  {
    unsigned int i=s.dataResult[0].size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!VehicleDataResultMarshaller::checkIntegrityConst(s.dataResult[0][i]))   return false;
    }
  }
  return true;
}

Json::Value SubscribeVehicleData_responseMarshaller::toJSON(const SubscribeVehicleData_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  if(e.info)
    json["info"]=Json::Value(*e.info);

  if(e.dataResult)
  {
    unsigned int sz=e.dataResult->size();
    json["dataResult"]=Json::Value(Json::arrayValue);
    json["dataResult"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["dataResult"][i]=VehicleDataResultMarshaller::toJSON(e.dataResult[0][i]);
  }

  return json;
}


bool SubscribeVehicleData_responseMarshaller::fromJSON(const Json::Value& json,SubscribeVehicleData_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  if(c.dataResult)  delete c.dataResult;
  c.dataResult=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("success"))  return false;
    {
      const Json::Value& j=json["success"];
      if(!j.isBool())  return false;
      c.success=j.asBool();
    }
    if(!json.isMember("resultCode"))  return false;
    {
      const Json::Value& j=json["resultCode"];
      if(!ResultMarshaller::fromJSON(j,c.resultCode))
        return false;
    }
    if(json.isMember("info"))
    {
      const Json::Value& j=json["info"];
      if(!j.isString())  return false;
      c.info=new std::string(j.asString());
    }
    if(json.isMember("dataResult"))
    {
      const Json::Value& j=json["dataResult"];
      if(!j.isArray())  return false;
      c.dataResult=new std::vector<VehicleDataResult>();
      c.dataResult->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        VehicleDataResult t;
        if(!VehicleDataResultMarshaller::fromJSON(j[i],t))
          return false;
        c.dataResult[0][i]=t;
      }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

