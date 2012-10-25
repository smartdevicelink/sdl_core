#include "../../include/JSONHandler/ALRPCObjects/GPSPublishedData.h"
#include "GPSDataMarshaller.h"

#include "GPSPublishedDataMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


bool GPSPublishedDataMarshaller::checkIntegrity(GPSPublishedData& s)
{
  return checkIntegrityConst(s);
}


bool GPSPublishedDataMarshaller::fromString(const std::string& s,GPSPublishedData& e)
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


const std::string GPSPublishedDataMarshaller::toString(const GPSPublishedData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GPSPublishedDataMarshaller::checkIntegrityConst(const GPSPublishedData& s)
{
  if(!GPSDataMarshaller::checkIntegrityConst(s.gpsData))  return false;
  return true;
}

Json::Value GPSPublishedDataMarshaller::toJSON(const GPSPublishedData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["gpsData"]=GPSDataMarshaller::toJSON(e.gpsData);

  json["gpsPositionValid"]=Json::Value(e.gpsPositionValid);


  return json;
}


bool GPSPublishedDataMarshaller::fromJSON(const Json::Value& json,GPSPublishedData& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("gpsData"))  return false;
    {
      const Json::Value& j=json["gpsData"];
      if(!GPSDataMarshaller::fromJSON(j,c.gpsData))
        return false;
    }
    if(!json.isMember("gpsPositionValid"))  return false;
    {
      const Json::Value& j=json["gpsPositionValid"];
      if(!j.isBool())  return false;
      c.gpsPositionValid=j.asBool();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

