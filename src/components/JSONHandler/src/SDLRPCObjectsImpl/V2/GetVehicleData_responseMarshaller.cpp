//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/GetVehicleData_response.h"
#include "GPSDataMarshaller.h"
#include "PRNDLMarshaller.h"
#include "ResultMarshaller.h"
#include "TireStatusMarshaller.h"

#include "GetVehicleData_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool GetVehicleData_responseMarshaller::checkIntegrity(GetVehicleData_response& s)
{
  return checkIntegrityConst(s);
}


bool GetVehicleData_responseMarshaller::fromString(const std::string& s,GetVehicleData_response& e)
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


const std::string GetVehicleData_responseMarshaller::toString(const GetVehicleData_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetVehicleData_responseMarshaller::checkIntegrityConst(const GetVehicleData_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.info && s.info->length()>1000)  return false;
  if(s.gps && !GPSDataMarshaller::checkIntegrityConst(*s.gps))  return false;
  if(s.speed && *s.speed>500)  return false;
  if(s.speed && *s.speed<-100)  return false;
  if(s.rpm && *s.rpm>20000)  return false;
  if(s.fuelLevel && *s.fuelLevel>100)  return false;
  if(s.fuelLevel && *s.fuelLevel<0)  return false;
  if(s.avgFuelEconomy && *s.avgFuelEconomy>7000)  return false;
  if(s.avgFuelEconomy && *s.avgFuelEconomy<0)  return false;
  if(s.batteryVoltage && *s.batteryVoltage>26)  return false;
  if(s.batteryVoltage && *s.batteryVoltage<0)  return false;
  if(s.externalTemperature && *s.externalTemperature>100)  return false;
  if(s.externalTemperature && *s.externalTemperature<-40)  return false;
  if(s.vin && s.vin->length()>17)  return false;
  if(s.prndl && !PRNDLMarshaller::checkIntegrityConst(*s.prndl))  return false;
  if(s.tirePressure && !TireStatusMarshaller::checkIntegrityConst(*s.tirePressure))  return false;
  if(s.batteryPackVoltage && *s.batteryPackVoltage>1000)  return false;
  if(s.batteryPackVoltage && *s.batteryPackVoltage<-1000)  return false;
  if(s.batteryPackCurrent && *s.batteryPackCurrent>1000)  return false;
  if(s.batteryPackCurrent && *s.batteryPackCurrent<-1000)  return false;
  if(s.batteryPackTemperature && *s.batteryPackTemperature>250)  return false;
  if(s.batteryPackTemperature && *s.batteryPackTemperature<-50)  return false;
  if(s.engineTorque && *s.engineTorque>55000)  return false;
  if(s.engineTorque && *s.engineTorque<-10000)  return false;
  if(s.odometer && *s.odometer>2000000)  return false;
  if(s.tripOdometer && *s.tripOdometer>2000000)  return false;
  if(s.satRadioESN && s.satRadioESN->length()>500)  return false;
  return true;
}

Json::Value GetVehicleData_responseMarshaller::toJSON(const GetVehicleData_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  if(e.info)
    json["info"]=Json::Value(*e.info);

  if(e.gps)
    json["gps"]=GPSDataMarshaller::toJSON(*e.gps);

  if(e.speed)
    json["speed"]=Json::Value(*e.speed);

  if(e.rpm)
    json["rpm"]=Json::Value(*e.rpm);

  if(e.fuelLevel)
    json["fuelLevel"]=Json::Value(*e.fuelLevel);

  if(e.avgFuelEconomy)
    json["avgFuelEconomy"]=Json::Value(*e.avgFuelEconomy);

  if(e.batteryVoltage)
    json["batteryVoltage"]=Json::Value(*e.batteryVoltage);

  if(e.externalTemperature)
    json["externalTemperature"]=Json::Value(*e.externalTemperature);

  if(e.vin)
    json["vin"]=Json::Value(*e.vin);

  if(e.prndl)
    json["prndl"]=PRNDLMarshaller::toJSON(*e.prndl);

  if(e.tirePressure)
    json["tirePressure"]=TireStatusMarshaller::toJSON(*e.tirePressure);

  if(e.batteryPackVoltage)
    json["batteryPackVoltage"]=Json::Value(*e.batteryPackVoltage);

  if(e.batteryPackCurrent)
    json["batteryPackCurrent"]=Json::Value(*e.batteryPackCurrent);

  if(e.batteryPackTemperature)
    json["batteryPackTemperature"]=Json::Value(*e.batteryPackTemperature);

  if(e.engineTorque)
    json["engineTorque"]=Json::Value(*e.engineTorque);

  if(e.odometer)
    json["odometer"]=Json::Value(*e.odometer);

  if(e.tripOdometer)
    json["tripOdometer"]=Json::Value(*e.tripOdometer);

  if(e.satRadioESN)
    json["satRadioESN"]=Json::Value(*e.satRadioESN);

  return json;
}


bool GetVehicleData_responseMarshaller::fromJSON(const Json::Value& json,GetVehicleData_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  if(c.gps)  delete c.gps;
  c.gps=0;

  if(c.speed)  delete c.speed;
  c.speed=0;

  if(c.rpm)  delete c.rpm;
  c.rpm=0;

  if(c.fuelLevel)  delete c.fuelLevel;
  c.fuelLevel=0;

  if(c.avgFuelEconomy)  delete c.avgFuelEconomy;
  c.avgFuelEconomy=0;

  if(c.batteryVoltage)  delete c.batteryVoltage;
  c.batteryVoltage=0;

  if(c.externalTemperature)  delete c.externalTemperature;
  c.externalTemperature=0;

  if(c.vin)  delete c.vin;
  c.vin=0;

  if(c.prndl)  delete c.prndl;
  c.prndl=0;

  if(c.tirePressure)  delete c.tirePressure;
  c.tirePressure=0;

  if(c.batteryPackVoltage)  delete c.batteryPackVoltage;
  c.batteryPackVoltage=0;

  if(c.batteryPackCurrent)  delete c.batteryPackCurrent;
  c.batteryPackCurrent=0;

  if(c.batteryPackTemperature)  delete c.batteryPackTemperature;
  c.batteryPackTemperature=0;

  if(c.engineTorque)  delete c.engineTorque;
  c.engineTorque=0;

  if(c.odometer)  delete c.odometer;
  c.odometer=0;

  if(c.tripOdometer)  delete c.tripOdometer;
  c.tripOdometer=0;

  if(c.satRadioESN)  delete c.satRadioESN;
  c.satRadioESN=0;

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
    if(json.isMember("gps"))
    {
      const Json::Value& j=json["gps"];
      c.gps=new GPSData();
      if(!GPSDataMarshaller::fromJSON(j,c.gps[0]))
        return false;
    }
    if(json.isMember("speed"))
    {
      const Json::Value& j=json["speed"];
      if(!j.isNumeric())  return false;
      c.speed=new float(j.asDouble());
    }
    if(json.isMember("rpm"))
    {
      const Json::Value& j=json["rpm"];
      if(!j.isInt())  return false;
      c.rpm=new unsigned int(j.asInt());
    }
    if(json.isMember("fuelLevel"))
    {
      const Json::Value& j=json["fuelLevel"];
      if(!j.isNumeric())  return false;
      c.fuelLevel=new float(j.asDouble());
    }
    if(json.isMember("avgFuelEconomy"))
    {
      const Json::Value& j=json["avgFuelEconomy"];
      if(!j.isNumeric())  return false;
      c.avgFuelEconomy=new float(j.asDouble());
    }
    if(json.isMember("batteryVoltage"))
    {
      const Json::Value& j=json["batteryVoltage"];
      if(!j.isNumeric())  return false;
      c.batteryVoltage=new float(j.asDouble());
    }
    if(json.isMember("externalTemperature"))
    {
      const Json::Value& j=json["externalTemperature"];
      if(!j.isNumeric())  return false;
      c.externalTemperature=new float(j.asDouble());
    }
    if(json.isMember("vin"))
    {
      const Json::Value& j=json["vin"];
      if(!j.isString())  return false;
      c.vin=new std::string(j.asString());
    }
    if(json.isMember("prndl"))
    {
      const Json::Value& j=json["prndl"];
      c.prndl=new PRNDL();
      if(!PRNDLMarshaller::fromJSON(j,c.prndl[0]))
        return false;
    }
    if(json.isMember("tirePressure"))
    {
      const Json::Value& j=json["tirePressure"];
      c.tirePressure=new TireStatus();
      if(!TireStatusMarshaller::fromJSON(j,c.tirePressure[0]))
        return false;
    }
    if(json.isMember("batteryPackVoltage"))
    {
      const Json::Value& j=json["batteryPackVoltage"];
      if(!j.isNumeric())  return false;
      c.batteryPackVoltage=new float(j.asDouble());
    }
    if(json.isMember("batteryPackCurrent"))
    {
      const Json::Value& j=json["batteryPackCurrent"];
      if(!j.isNumeric())  return false;
      c.batteryPackCurrent=new float(j.asDouble());
    }
    if(json.isMember("batteryPackTemperature"))
    {
      const Json::Value& j=json["batteryPackTemperature"];
      if(!j.isInt())  return false;
      c.batteryPackTemperature=new int(j.asInt());
    }
    if(json.isMember("engineTorque"))
    {
      const Json::Value& j=json["engineTorque"];
      if(!j.isInt())  return false;
      c.engineTorque=new int(j.asInt());
    }
    if(json.isMember("odometer"))
    {
      const Json::Value& j=json["odometer"];
      if(!j.isInt())  return false;
      c.odometer=new unsigned int(j.asInt());
    }
    if(json.isMember("tripOdometer"))
    {
      const Json::Value& j=json["tripOdometer"];
      if(!j.isInt())  return false;
      c.tripOdometer=new unsigned int(j.asInt());
    }
    if(json.isMember("satRadioESN"))
    {
      const Json::Value& j=json["satRadioESN"];
      if(!j.isString())  return false;
      c.satRadioESN=new std::string(j.asString());
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

