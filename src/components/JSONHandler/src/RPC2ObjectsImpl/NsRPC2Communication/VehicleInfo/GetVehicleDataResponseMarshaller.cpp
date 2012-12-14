#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleDataResponse.h"
#include "../src/ALRPCObjectsImpl/V1/GPSDataMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/PRNDLMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/TireStatusMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetVehicleDataResponseMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VehicleInfo;

bool GetVehicleDataResponseMarshaller::checkIntegrity(GetVehicleDataResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetVehicleDataResponseMarshaller::fromString(const std::string& s,GetVehicleDataResponse& e)
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


const std::string GetVehicleDataResponseMarshaller::toString(const GetVehicleDataResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetVehicleDataResponseMarshaller::checkIntegrityConst(const GetVehicleDataResponse& s)
{
  if(s.gps && (!NsAppLinkRPC::GPSDataMarshaller::checkIntegrityConst(s.gps[0])))  return false;

  if(s.speed && (s.speed[0]<-100 || s.speed[0]>500))  return false;

  if(s.rpm && (s.rpm[0]>20000))  return false;

  if(s.fuelLevel && (s.fuelLevel[0]<0 || s.fuelLevel[0]>100))  return false;

  if(s.avgFuelEconomy && (s.avgFuelEconomy[0]<0 || s.avgFuelEconomy[0]>7000))  return false;

  if(s.batteryVoltage && (s.batteryVoltage[0]<0 || s.batteryVoltage[0]>26))  return false;

  if(s.externalTemperature && (s.externalTemperature[0]<-40 || s.externalTemperature[0]>100))  return false;

  if(s.vin && (s.vin[0].length()>17))  return false;

  if(s.prndl && (!NsAppLinkRPC::PRNDLMarshaller::checkIntegrityConst(s.prndl[0])))  return false;

  if(s.tirePressure && (!NsAppLinkRPC::TireStatusMarshaller::checkIntegrityConst(s.tirePressure[0])))  return false;

  if(s.batteryPackVoltage && (s.batteryPackVoltage[0]<-1000 || s.batteryPackVoltage[0]>1000))  return false;

  if(s.batteryPackCurrent && (s.batteryPackCurrent[0]<-1000 || s.batteryPackCurrent[0]>1000))  return false;

  if(s.batteryPackTemperature && (s.batteryPackTemperature[0]<-50 || s.batteryPackTemperature[0]>250))  return false;

  if(s.engineTorque && (s.engineTorque[0]<-10000 || s.engineTorque[0]>55000))  return false;

  if(s.odometer && (s.odometer[0]>2000000))  return false;

  if(s.tripOdometer && (s.tripOdometer[0]>2000000))  return false;

  if(s.satRadioESN && (s.satRadioESN[0].length()>500))  return false;

  return true;
}


Json::Value GetVehicleDataResponseMarshaller::toJSON(const GetVehicleDataResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("AppLinkCore.GetVehicleDataResponse");

  if(e.gps)
    json["result"]["gps"]=NsAppLinkRPC::GPSDataMarshaller::toJSON(e.gps[0]);;
  if(e.speed)
    json["result"]["speed"]=Json::Value(e.speed[0]);;
  if(e.rpm)
    json["result"]["rpm"]=Json::Value(e.rpm[0]);;
  if(e.fuelLevel)
    json["result"]["fuelLevel"]=Json::Value(e.fuelLevel[0]);;
  if(e.avgFuelEconomy)
    json["result"]["avgFuelEconomy"]=Json::Value(e.avgFuelEconomy[0]);;
  if(e.batteryVoltage)
    json["result"]["batteryVoltage"]=Json::Value(e.batteryVoltage[0]);;
  if(e.externalTemperature)
    json["result"]["externalTemperature"]=Json::Value(e.externalTemperature[0]);;
  if(e.vin)
    json["result"]["vin"]=Json::Value(e.vin[0]);;
  if(e.prndl)
    json["result"]["prndl"]=NsAppLinkRPC::PRNDLMarshaller::toJSON(e.prndl[0]);;
  if(e.tirePressure)
    json["result"]["tirePressure"]=NsAppLinkRPC::TireStatusMarshaller::toJSON(e.tirePressure[0]);;
  if(e.batteryPackVoltage)
    json["result"]["batteryPackVoltage"]=Json::Value(e.batteryPackVoltage[0]);;
  if(e.batteryPackCurrent)
    json["result"]["batteryPackCurrent"]=Json::Value(e.batteryPackCurrent[0]);;
  if(e.batteryPackTemperature)
    json["result"]["batteryPackTemperature"]=Json::Value(e.batteryPackTemperature[0]);;
  if(e.engineTorque)
    json["result"]["engineTorque"]=Json::Value(e.engineTorque[0]);;
  if(e.odometer)
    json["result"]["odometer"]=Json::Value(e.odometer[0]);;
  if(e.tripOdometer)
    json["result"]["tripOdometer"]=Json::Value(e.tripOdometer[0]);;
  if(e.satRadioESN)
    json["result"]["satRadioESN"]=Json::Value(e.satRadioESN[0]);;
  return json;
}


bool GetVehicleDataResponseMarshaller::fromJSON(const Json::Value& json,GetVehicleDataResponse& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("result")) return false;

    Json::Value js=json["result"];
    if(!js.isObject())  return false;

    NsAppLinkRPC::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("AppLinkCore.GetVehicleDataResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(c.gps)  delete c.gps;
    c.gps=0;
    if(js.isMember("gps"))
    {
      c.gps=new NsAppLinkRPC::GPSData();
      if(!NsAppLinkRPC::GPSDataMarshaller::fromJSON(js["gps"],c.gps[0]))  return false;
    }

    if(c.speed)  delete c.speed;
    c.speed=0;
    if(js.isMember("speed"))
    {
      if(!js["speed"].isNumeric())  return false;
      c.speed=new float();
      c.speed[0]=js["speed"].asDouble();
      if(c.speed[0]<-100 || c.speed[0]>500)  return false;

    }

    if(c.rpm)  delete c.rpm;
    c.rpm=0;
    if(js.isMember("rpm"))
    {
      if(!js["rpm"].isInt())  return false;
      c.rpm=new unsigned int();
      c.rpm[0]=js["rpm"].asInt();
      if(c.rpm[0]>20000)  return false;

    }

    if(c.fuelLevel)  delete c.fuelLevel;
    c.fuelLevel=0;
    if(js.isMember("fuelLevel"))
    {
      if(!js["fuelLevel"].isNumeric())  return false;
      c.fuelLevel=new float();
      c.fuelLevel[0]=js["fuelLevel"].asDouble();
      if(c.fuelLevel[0]<0 || c.fuelLevel[0]>100)  return false;

    }

    if(c.avgFuelEconomy)  delete c.avgFuelEconomy;
    c.avgFuelEconomy=0;
    if(js.isMember("avgFuelEconomy"))
    {
      if(!js["avgFuelEconomy"].isNumeric())  return false;
      c.avgFuelEconomy=new float();
      c.avgFuelEconomy[0]=js["avgFuelEconomy"].asDouble();
      if(c.avgFuelEconomy[0]<0 || c.avgFuelEconomy[0]>7000)  return false;

    }

    if(c.batteryVoltage)  delete c.batteryVoltage;
    c.batteryVoltage=0;
    if(js.isMember("batteryVoltage"))
    {
      if(!js["batteryVoltage"].isNumeric())  return false;
      c.batteryVoltage=new float();
      c.batteryVoltage[0]=js["batteryVoltage"].asDouble();
      if(c.batteryVoltage[0]<0 || c.batteryVoltage[0]>26)  return false;

    }

    if(c.externalTemperature)  delete c.externalTemperature;
    c.externalTemperature=0;
    if(js.isMember("externalTemperature"))
    {
      if(!js["externalTemperature"].isNumeric())  return false;
      c.externalTemperature=new float();
      c.externalTemperature[0]=js["externalTemperature"].asDouble();
      if(c.externalTemperature[0]<-40 || c.externalTemperature[0]>100)  return false;

    }

    if(c.vin)  delete c.vin;
    c.vin=0;
    if(js.isMember("vin"))
    {
      if(!js["vin"].isString())  return false;
      c.vin=new std::string();
      c.vin[0]=js["vin"].asString();
      if(c.vin[0].length()>17)  return false;

    }

    if(c.prndl)  delete c.prndl;
    c.prndl=0;
    if(js.isMember("prndl"))
    {
      c.prndl=new NsAppLinkRPC::PRNDL();
      if(!NsAppLinkRPC::PRNDLMarshaller::fromJSON(js["prndl"],c.prndl[0]))  return false;
    }

    if(c.tirePressure)  delete c.tirePressure;
    c.tirePressure=0;
    if(js.isMember("tirePressure"))
    {
      c.tirePressure=new NsAppLinkRPC::TireStatus();
      if(!NsAppLinkRPC::TireStatusMarshaller::fromJSON(js["tirePressure"],c.tirePressure[0]))  return false;
    }

    if(c.batteryPackVoltage)  delete c.batteryPackVoltage;
    c.batteryPackVoltage=0;
    if(js.isMember("batteryPackVoltage"))
    {
      if(!js["batteryPackVoltage"].isNumeric())  return false;
      c.batteryPackVoltage=new float();
      c.batteryPackVoltage[0]=js["batteryPackVoltage"].asDouble();
      if(c.batteryPackVoltage[0]<-1000 || c.batteryPackVoltage[0]>1000)  return false;

    }

    if(c.batteryPackCurrent)  delete c.batteryPackCurrent;
    c.batteryPackCurrent=0;
    if(js.isMember("batteryPackCurrent"))
    {
      if(!js["batteryPackCurrent"].isNumeric())  return false;
      c.batteryPackCurrent=new float();
      c.batteryPackCurrent[0]=js["batteryPackCurrent"].asDouble();
      if(c.batteryPackCurrent[0]<-1000 || c.batteryPackCurrent[0]>1000)  return false;

    }

    if(c.batteryPackTemperature)  delete c.batteryPackTemperature;
    c.batteryPackTemperature=0;
    if(js.isMember("batteryPackTemperature"))
    {
      if(!js["batteryPackTemperature"].isInt())  return false;
      c.batteryPackTemperature=new int();
      c.batteryPackTemperature[0]=js["batteryPackTemperature"].asInt();
      if(c.batteryPackTemperature[0]<-50 || c.batteryPackTemperature[0]>250)  return false;

    }

    if(c.engineTorque)  delete c.engineTorque;
    c.engineTorque=0;
    if(js.isMember("engineTorque"))
    {
      if(!js["engineTorque"].isInt())  return false;
      c.engineTorque=new int();
      c.engineTorque[0]=js["engineTorque"].asInt();
      if(c.engineTorque[0]<-10000 || c.engineTorque[0]>55000)  return false;

    }

    if(c.odometer)  delete c.odometer;
    c.odometer=0;
    if(js.isMember("odometer"))
    {
      if(!js["odometer"].isInt())  return false;
      c.odometer=new unsigned int();
      c.odometer[0]=js["odometer"].asInt();
      if(c.odometer[0]>2000000)  return false;

    }

    if(c.tripOdometer)  delete c.tripOdometer;
    c.tripOdometer=0;
    if(js.isMember("tripOdometer"))
    {
      if(!js["tripOdometer"].isInt())  return false;
      c.tripOdometer=new unsigned int();
      c.tripOdometer[0]=js["tripOdometer"].asInt();
      if(c.tripOdometer[0]>2000000)  return false;

    }

    if(c.satRadioESN)  delete c.satRadioESN;
    c.satRadioESN=0;
    if(js.isMember("satRadioESN"))
    {
      if(!js["satRadioESN"].isString())  return false;
      c.satRadioESN=new std::string();
      c.satRadioESN[0]=js["satRadioESN"].asString();
      if(c.satRadioESN[0].length()>500)  return false;

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
