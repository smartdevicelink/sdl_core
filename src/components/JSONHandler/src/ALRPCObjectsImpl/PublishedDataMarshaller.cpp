#include <cstring>

#include "../../include/JSONHandler/ALRPCObjects/PublishedData.h"
#include "GPSPublishedDataMarshaller.h"
#include "PRNDLMarshaller.h"
#include "TireStatusMarshaller.h"

#include "PublishedDataMarshaller.h"
#include "PublishedDataMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/

bool PublishedDataMarshaller::checkIntegrity(PublishedData& s)
{
  return checkIntegrityConst(s);
}

const PublishedData::eDiscriminator PublishedDataMarshaller::getIndex(const char* s)
{
  if(!s)
    return PublishedData::INVALID_ENUM;
  const struct PerfectHashTable* p=PublishedData_intHash::getPointer(s,strlen(s));
  return p ? static_cast<PublishedData::eDiscriminator>(p->idx) : PublishedData::INVALID_ENUM;
}


bool PublishedDataMarshaller::fromString(const std::string& s,PublishedData& e)
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


const std::string PublishedDataMarshaller::toString(const PublishedData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PublishedDataMarshaller::checkIntegrityConst(const PublishedData& s)
{
  switch(s.mInternal.mDiscriminator)
  {
    case PublishedData::INVALID_ENUM:
      return false;
    case PublishedData::avgFuelEconomy:
      if(s.mInternal.avgFuelEconomy>7000)  return false;
      if(s.mInternal.avgFuelEconomy<0)  return false;
      return true;
    case PublishedData::batteryPackCurrent:
      if(s.mInternal.batteryPackCurrent>1000)  return false;
      if(s.mInternal.batteryPackCurrent<-1000)  return false;
      return true;
    case PublishedData::batteryPackTemperature:
      if(s.mInternal.batteryPackTemperature>250)  return false;
      return true;
    case PublishedData::batteryPackVoltage:
      if(s.mInternal.batteryPackVoltage>1000)  return false;
      if(s.mInternal.batteryPackVoltage<-1000)  return false;
      return true;
    case PublishedData::batteryVoltage:
      if(s.mInternal.batteryVoltage>26)  return false;
      if(s.mInternal.batteryVoltage<0)  return false;
      return true;
    case PublishedData::engineTorque:
      if(s.mInternal.engineTorque>55000)  return false;
      return true;
    case PublishedData::externalTemperature:
      if(s.mInternal.externalTemperature>100)  return false;
      if(s.mInternal.externalTemperature<-40)  return false;
      return true;
    case PublishedData::fuelLevel:
      if(s.mInternal.fuelLevel>100)  return false;
      if(s.mInternal.fuelLevel<0)  return false;
      return true;
    case PublishedData::genericbinary:
      return true;
    case PublishedData::gps:
      return GPSPublishedDataMarshaller::checkIntegrityConst(s.mInternal.gps);
    case PublishedData::odometer:
      if(s.mInternal.odometer>2000000)  return false;
      return true;
    case PublishedData::prndl:
      return PRNDLMarshaller::checkIntegrityConst(s.mInternal.prndl);
    case PublishedData::rpm:
      if(s.mInternal.rpm>20000)  return false;
      return true;
    case PublishedData::speed:
      if(s.mInternal.speed>500)  return false;
      if(s.mInternal.speed<-100)  return false;
      return true;
    case PublishedData::tirePressure:
      return TireStatusMarshaller::checkIntegrityConst(s.mInternal.tirePressure);
    case PublishedData::tripOdometer:
      if(s.mInternal.tripOdometer>2000000)  return false;
      return true;
    case PublishedData::vin:
      if(s.mInternal.vin.length()>17)  return false;
      return true;
  }
  return false;
}

Json::Value PublishedDataMarshaller::toJSON(const PublishedData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  switch(e.mInternal.mDiscriminator)
  {
    case PublishedData::INVALID_ENUM:
      break;

    case PublishedData::avgFuelEconomy:
      json["avgFuelEconomy"]=Json::Value(e.mInternal.avgFuelEconomy);
      break;
    case PublishedData::batteryPackCurrent:
      json["batteryPackCurrent"]=Json::Value(e.mInternal.batteryPackCurrent);
      break;
    case PublishedData::batteryPackTemperature:
      json["batteryPackTemperature"]=Json::Value(e.mInternal.batteryPackTemperature);
      break;
    case PublishedData::batteryPackVoltage:
      json["batteryPackVoltage"]=Json::Value(e.mInternal.batteryPackVoltage);
      break;
    case PublishedData::batteryVoltage:
      json["batteryVoltage"]=Json::Value(e.mInternal.batteryVoltage);
      break;
    case PublishedData::engineTorque:
      json["engineTorque"]=Json::Value(e.mInternal.engineTorque);
      break;
    case PublishedData::externalTemperature:
      json["externalTemperature"]=Json::Value(e.mInternal.externalTemperature);
      break;
    case PublishedData::fuelLevel:
      json["fuelLevel"]=Json::Value(e.mInternal.fuelLevel);
      break;
    case PublishedData::genericbinary:
      json["genericbinary"]=Json::Value(e.mInternal.genericbinary);
      break;
    case PublishedData::gps:
      json["gps"]=GPSPublishedDataMarshaller::toJSON(e.mInternal.gps);
      break;
    case PublishedData::odometer:
      json["odometer"]=Json::Value(e.mInternal.odometer);
      break;
    case PublishedData::prndl:
      json["prndl"]=PRNDLMarshaller::toJSON(e.mInternal.prndl);
      break;
    case PublishedData::rpm:
      json["rpm"]=Json::Value(e.mInternal.rpm);
      break;
    case PublishedData::speed:
      json["speed"]=Json::Value(e.mInternal.speed);
      break;
    case PublishedData::tirePressure:
      json["tirePressure"]=TireStatusMarshaller::toJSON(e.mInternal.tirePressure);
      break;
    case PublishedData::tripOdometer:
      json["tripOdometer"]=Json::Value(e.mInternal.tripOdometer);
      break;
    case PublishedData::vin:
      json["vin"]=Json::Value(e.mInternal.vin);
      break;
  }

  return json;
}

bool PublishedDataMarshaller::fromJSON(const Json::Value& json,PublishedData& c)
{
  c.mInternal.mDiscriminator=PublishedData::INVALID_ENUM;

  try
  {
    if(!json.isObject())  return false;

    if(json.isMember("avgFuelEconomy"))
    {
      const Json::Value& j=json["avgFuelEconomy"];
      if(!j.isDouble())  return false;
      c.mInternal.avgFuelEconomy=j.asDouble();
      c.mInternal.mDiscriminator=PublishedData::avgFuelEconomy;
      goto leave;
    }
    if(json.isMember("batteryPackCurrent"))
    {
      const Json::Value& j=json["batteryPackCurrent"];
      if(!j.isDouble())  return false;
      c.mInternal.batteryPackCurrent=j.asDouble();
      c.mInternal.mDiscriminator=PublishedData::batteryPackCurrent;
      goto leave;
    }
    if(json.isMember("batteryPackTemperature"))
    {
      const Json::Value& j=json["batteryPackTemperature"];
      if(!j.isInt())  return false;
      c.mInternal.batteryPackTemperature=j.asInt();
      c.mInternal.mDiscriminator=PublishedData::batteryPackTemperature;
      goto leave;
    }
    if(json.isMember("batteryPackVoltage"))
    {
      const Json::Value& j=json["batteryPackVoltage"];
      if(!j.isDouble())  return false;
      c.mInternal.batteryPackVoltage=j.asDouble();
      c.mInternal.mDiscriminator=PublishedData::batteryPackVoltage;
      goto leave;
    }
    if(json.isMember("batteryVoltage"))
    {
      const Json::Value& j=json["batteryVoltage"];
      if(!j.isDouble())  return false;
      c.mInternal.batteryVoltage=j.asDouble();
      c.mInternal.mDiscriminator=PublishedData::batteryVoltage;
      goto leave;
    }
    if(json.isMember("engineTorque"))
    {
      const Json::Value& j=json["engineTorque"];
      if(!j.isInt())  return false;
      c.mInternal.engineTorque=j.asInt();
      c.mInternal.mDiscriminator=PublishedData::engineTorque;
      goto leave;
    }
    if(json.isMember("externalTemperature"))
    {
      const Json::Value& j=json["externalTemperature"];
      if(!j.isDouble())  return false;
      c.mInternal.externalTemperature=j.asDouble();
      c.mInternal.mDiscriminator=PublishedData::externalTemperature;
      goto leave;
    }
    if(json.isMember("fuelLevel"))
    {
      const Json::Value& j=json["fuelLevel"];
      if(!j.isDouble())  return false;
      c.mInternal.fuelLevel=j.asDouble();
      c.mInternal.mDiscriminator=PublishedData::fuelLevel;
      goto leave;
    }
    if(json.isMember("genericbinary"))
    {
      const Json::Value& j=json["genericbinary"];
      if(!j.isString())  return false;
      c.mInternal.genericbinary=j.asString();
      c.mInternal.mDiscriminator=PublishedData::genericbinary;
      goto leave;
    }
    if(json.isMember("gps"))
    {
      const Json::Value& j=json["gps"];
      if(!GPSPublishedDataMarshaller::fromJSON(j,c.mInternal.gps))
        return false;
      c.mInternal.mDiscriminator=PublishedData::gps;
      goto leave;
    }
    if(json.isMember("odometer"))
    {
      const Json::Value& j=json["odometer"];
      if(!j.isInt())  return false;
      c.mInternal.odometer=j.asInt();
      c.mInternal.mDiscriminator=PublishedData::odometer;
      goto leave;
    }
    if(json.isMember("prndl"))
    {
      const Json::Value& j=json["prndl"];
      if(!PRNDLMarshaller::fromJSON(j,c.mInternal.prndl))
        return false;
      c.mInternal.mDiscriminator=PublishedData::prndl;
      goto leave;
    }
    if(json.isMember("rpm"))
    {
      const Json::Value& j=json["rpm"];
      if(!j.isInt())  return false;
      c.mInternal.rpm=j.asInt();
      c.mInternal.mDiscriminator=PublishedData::rpm;
      goto leave;
    }
    if(json.isMember("speed"))
    {
      const Json::Value& j=json["speed"];
      if(!j.isDouble())  return false;
      c.mInternal.speed=j.asDouble();
      c.mInternal.mDiscriminator=PublishedData::speed;
      goto leave;
    }
    if(json.isMember("tirePressure"))
    {
      const Json::Value& j=json["tirePressure"];
      if(!TireStatusMarshaller::fromJSON(j,c.mInternal.tirePressure))
        return false;
      c.mInternal.mDiscriminator=PublishedData::tirePressure;
      goto leave;
    }
    if(json.isMember("tripOdometer"))
    {
      const Json::Value& j=json["tripOdometer"];
      if(!j.isInt())  return false;
      c.mInternal.tripOdometer=j.asInt();
      c.mInternal.mDiscriminator=PublishedData::tripOdometer;
      goto leave;
    }
    if(json.isMember("vin"))
    {
      const Json::Value& j=json["vin"];
      if(!j.isString())  return false;
      c.mInternal.vin=j.asString();
      c.mInternal.mDiscriminator=PublishedData::vin;
      goto leave;
    }

  }
  catch(...)
  {
    c.mInternal.mDiscriminator=PublishedData::INVALID_ENUM;
    return false;
  }
  c.mInternal.mDiscriminator=PublishedData::INVALID_ENUM;
  return false;
leave:
  return checkIntegrity(c);
}

const PerfectHashTable PublishedDataMarshaller::mHashTable[17]=
{
  {"avgFuelEconomy",0},
  {"batteryPackCurrent",1},
  {"batteryPackTemperature",2},
  {"batteryPackVoltage",3},
  {"batteryVoltage",4},
  {"engineTorque",5},
  {"externalTemperature",6},
  {"fuelLevel",7},
  {"genericbinary",8},
  {"gps",9},
  {"odometer",10},
  {"prndl",11},
  {"rpm",12},
  {"speed",13},
  {"tirePressure",14},
  {"tripOdometer",15},
  {"vin",16}
};
