#include "../include/JSONHandler/ALRPCObjects/VehicleDataResult.h"
#include "VehicleDataResultCodeMarshaller.h"
#include "VehicleDataTypeMarshaller.h"

#include "VehicleDataResultMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool VehicleDataResultMarshaller::checkIntegrity(VehicleDataResult& s)
{
  return checkIntegrityConst(s);
}


bool VehicleDataResultMarshaller::fromString(const std::string& s,VehicleDataResult& e)
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


const std::string VehicleDataResultMarshaller::toString(const VehicleDataResult& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool VehicleDataResultMarshaller::checkIntegrityConst(const VehicleDataResult& s)
{
  if(!VehicleDataTypeMarshaller::checkIntegrityConst(s.dataType))  return false;
  if(!VehicleDataResultCodeMarshaller::checkIntegrityConst(s.resultCode))  return false;
  return true;
}

Json::Value VehicleDataResultMarshaller::toJSON(const VehicleDataResult& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["dataType"]=VehicleDataTypeMarshaller::toJSON(e.dataType);

  json["resultCode"]=VehicleDataResultCodeMarshaller::toJSON(e.resultCode);


  return json;
}


bool VehicleDataResultMarshaller::fromJSON(const Json::Value& json,VehicleDataResult& c)
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
    if(!json.isMember("resultCode"))  return false;
    {
      const Json::Value& j=json["resultCode"];
      if(!VehicleDataResultCodeMarshaller::fromJSON(j,c.resultCode))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

