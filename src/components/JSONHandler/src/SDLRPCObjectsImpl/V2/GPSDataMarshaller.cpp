//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/GPSData.h"
#include "CompassDirectionMarshaller.h"
#include "DimensionMarshaller.h"

#include "GPSDataMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool GPSDataMarshaller::checkIntegrity(GPSData& s)
{
  return checkIntegrityConst(s);
}


bool GPSDataMarshaller::fromString(const std::string& s,GPSData& e)
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


const std::string GPSDataMarshaller::toString(const GPSData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GPSDataMarshaller::checkIntegrityConst(const GPSData& s)
{
  if(s.altitude>10000)  return false;
  if(s.altitude<-10000)  return false;
  if(!CompassDirectionMarshaller::checkIntegrityConst(s.compassDirection))  return false;
  if(!DimensionMarshaller::checkIntegrityConst(s.dimension))  return false;
  if(s.hdop>31)  return false;
  if(s.heading>360)  return false;
  if(s.latitudeDegrees>1000000000)  return false;
  if(s.latitudeDegrees<-1000000000)  return false;
  if(s.longitudeDegrees>1000000000)  return false;
  if(s.longitudeDegrees<-1000000000)  return false;
  if(s.pdop>31)  return false;
  if(s.satellites>31)  return false;
  if(s.speed>400)  return false;
  if(s.utcDay>31)  return false;
  if(s.utcDay<1)  return false;
  if(s.utcHours>23)  return false;
  if(s.utcMinutes>59)  return false;
  if(s.utcMonth>12)  return false;
  if(s.utcMonth<1)  return false;
  if(s.utcSeconds>59)  return false;
  if(s.utcYear>2100)  return false;
  if(s.utcYear<2010)  return false;
  if(s.vdop>31)  return false;
  return true;
}

Json::Value GPSDataMarshaller::toJSON(const GPSData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["actual"]=Json::Value(e.actual);

  json["altitude"]=Json::Value(e.altitude);

  json["compassDirection"]=CompassDirectionMarshaller::toJSON(e.compassDirection);

  json["dimension"]=DimensionMarshaller::toJSON(e.dimension);

  json["hdop"]=Json::Value(e.hdop);

  json["heading"]=Json::Value(e.heading);

  json["latitudeDegrees"]=Json::Value(e.latitudeDegrees);

  json["longitudeDegrees"]=Json::Value(e.longitudeDegrees);

  json["pdop"]=Json::Value(e.pdop);

  json["satellites"]=Json::Value(e.satellites);

  json["speed"]=Json::Value(e.speed);

  json["utcDay"]=Json::Value(e.utcDay);

  json["utcHours"]=Json::Value(e.utcHours);

  json["utcMinutes"]=Json::Value(e.utcMinutes);

  json["utcMonth"]=Json::Value(e.utcMonth);

  json["utcSeconds"]=Json::Value(e.utcSeconds);

  json["utcYear"]=Json::Value(e.utcYear);

  json["vdop"]=Json::Value(e.vdop);


  return json;
}


bool GPSDataMarshaller::fromJSON(const Json::Value& json,GPSData& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("actual"))  return false;
    {
      const Json::Value& j=json["actual"];
      if(!j.isBool())  return false;
      c.actual=j.asBool();
    }
    if(!json.isMember("altitude"))  return false;
    {
      const Json::Value& j=json["altitude"];
      if(!j.isInt())  return false;
      c.altitude=j.asInt();
    }
    if(!json.isMember("compassDirection"))  return false;
    {
      const Json::Value& j=json["compassDirection"];
      if(!CompassDirectionMarshaller::fromJSON(j,c.compassDirection))
        return false;
    }
    if(!json.isMember("dimension"))  return false;
    {
      const Json::Value& j=json["dimension"];
      if(!DimensionMarshaller::fromJSON(j,c.dimension))
        return false;
    }
    if(!json.isMember("hdop"))  return false;
    {
      const Json::Value& j=json["hdop"];
      if(!j.isInt())  return false;
      c.hdop=j.asInt();
    }
    if(!json.isMember("heading"))  return false;
    {
      const Json::Value& j=json["heading"];
      if(!j.isInt())  return false;
      c.heading=j.asInt();
    }
    if(!json.isMember("latitudeDegrees"))  return false;
    {
      const Json::Value& j=json["latitudeDegrees"];
      if(!j.isInt())  return false;
      c.latitudeDegrees=j.asInt();
    }
    if(!json.isMember("longitudeDegrees"))  return false;
    {
      const Json::Value& j=json["longitudeDegrees"];
      if(!j.isInt())  return false;
      c.longitudeDegrees=j.asInt();
    }
    if(!json.isMember("pdop"))  return false;
    {
      const Json::Value& j=json["pdop"];
      if(!j.isInt())  return false;
      c.pdop=j.asInt();
    }
    if(!json.isMember("satellites"))  return false;
    {
      const Json::Value& j=json["satellites"];
      if(!j.isInt())  return false;
      c.satellites=j.asInt();
    }
    if(!json.isMember("speed"))  return false;
    {
      const Json::Value& j=json["speed"];
      if(!j.isInt())  return false;
      c.speed=j.asInt();
    }
    if(!json.isMember("utcDay"))  return false;
    {
      const Json::Value& j=json["utcDay"];
      if(!j.isInt())  return false;
      c.utcDay=j.asInt();
    }
    if(!json.isMember("utcHours"))  return false;
    {
      const Json::Value& j=json["utcHours"];
      if(!j.isInt())  return false;
      c.utcHours=j.asInt();
    }
    if(!json.isMember("utcMinutes"))  return false;
    {
      const Json::Value& j=json["utcMinutes"];
      if(!j.isInt())  return false;
      c.utcMinutes=j.asInt();
    }
    if(!json.isMember("utcMonth"))  return false;
    {
      const Json::Value& j=json["utcMonth"];
      if(!j.isInt())  return false;
      c.utcMonth=j.asInt();
    }
    if(!json.isMember("utcSeconds"))  return false;
    {
      const Json::Value& j=json["utcSeconds"];
      if(!j.isInt())  return false;
      c.utcSeconds=j.asInt();
    }
    if(!json.isMember("utcYear"))  return false;
    {
      const Json::Value& j=json["utcYear"];
      if(!j.isInt())  return false;
      c.utcYear=j.asInt();
    }
    if(!json.isMember("vdop"))  return false;
    {
      const Json::Value& j=json["vdop"];
      if(!j.isInt())  return false;
      c.vdop=j.asInt();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

