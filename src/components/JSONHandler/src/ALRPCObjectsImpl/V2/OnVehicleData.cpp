#include "../include/JSONHandler/ALRPCObjects/V2/OnVehicleData.h"
#include "OnVehicleDataMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "GPSDataMarshaller.h"
#include "PRNDLMarshaller.h"
#include "TireStatusMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
OnVehicleData& OnVehicleData::operator =(const OnVehicleData& c)
{
  gps= c.gps ? new GPSData(c.gps[0]) : 0;
  speed= c.speed ? new float(c.speed[0]) : 0;
  rpm= c.rpm ? new unsigned int(c.rpm[0]) : 0;
  fuelLevel= c.fuelLevel ? new float(c.fuelLevel[0]) : 0;
  avgFuelEconomy= c.avgFuelEconomy ? new float(c.avgFuelEconomy[0]) : 0;
  batteryVoltage= c.batteryVoltage ? new float(c.batteryVoltage[0]) : 0;
  externalTemperature= c.externalTemperature ? new float(c.externalTemperature[0]) : 0;
  vin= c.vin ? new std::string(c.vin[0]) : 0;
  prndl= c.prndl ? new PRNDL(c.prndl[0]) : 0;
  tirePressure= c.tirePressure ? new TireStatus(c.tirePressure[0]) : 0;
  batteryPackVoltage= c.batteryPackVoltage ? new float(c.batteryPackVoltage[0]) : 0;
  batteryPackCurrent= c.batteryPackCurrent ? new float(c.batteryPackCurrent[0]) : 0;
  batteryPackTemperature= c.batteryPackTemperature ? new int(c.batteryPackTemperature[0]) : 0;
  engineTorque= c.engineTorque ? new int(c.engineTorque[0]) : 0;
  odometer= c.odometer ? new unsigned int(c.odometer[0]) : 0;
  tripOdometer= c.tripOdometer ? new unsigned int(c.tripOdometer[0]) : 0;
  genericbinary= c.genericbinary ? new std::string(c.genericbinary[0]) : 0;

  return *this;
}


OnVehicleData::~OnVehicleData(void)
{
  if(gps)
    delete gps;
  if(speed)
    delete speed;
  if(rpm)
    delete rpm;
  if(fuelLevel)
    delete fuelLevel;
  if(avgFuelEconomy)
    delete avgFuelEconomy;
  if(batteryVoltage)
    delete batteryVoltage;
  if(externalTemperature)
    delete externalTemperature;
  if(vin)
    delete vin;
  if(prndl)
    delete prndl;
  if(tirePressure)
    delete tirePressure;
  if(batteryPackVoltage)
    delete batteryPackVoltage;
  if(batteryPackCurrent)
    delete batteryPackCurrent;
  if(batteryPackTemperature)
    delete batteryPackTemperature;
  if(engineTorque)
    delete engineTorque;
  if(odometer)
    delete odometer;
  if(tripOdometer)
    delete tripOdometer;
  if(genericbinary)
    delete genericbinary;
}


OnVehicleData::OnVehicleData(const OnVehicleData& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool OnVehicleData::checkIntegrity(void)
{
  return OnVehicleDataMarshaller::checkIntegrity(*this);
}


OnVehicleData::OnVehicleData(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      gps(0),
    speed(0),
    rpm(0),
    fuelLevel(0),
    avgFuelEconomy(0),
    batteryVoltage(0),
    externalTemperature(0),
    vin(0),
    prndl(0),
    tirePressure(0),
    batteryPackVoltage(0),
    batteryPackCurrent(0),
    batteryPackTemperature(0),
    engineTorque(0),
    odometer(0),
    tripOdometer(0),
    genericbinary(0)
{
}



bool OnVehicleData::set_gps(const GPSData& gps_)
{
  if(!GPSDataMarshaller::checkIntegrityConst(gps_))   return false;
  delete gps;
  gps=0;

  gps=new GPSData(gps_);
  return true;
}

void OnVehicleData::reset_gps(void)
{
  if(gps)
    delete gps;
  gps=0;
}

bool OnVehicleData::set_speed(float speed_)
{
  if(speed_>500)  return false;
  if(speed_<-100)  return false;
  delete speed;
  speed=0;

  speed=new float(speed_);
  return true;
}

void OnVehicleData::reset_speed(void)
{
  if(speed)
    delete speed;
  speed=0;
}

bool OnVehicleData::set_rpm(unsigned int rpm_)
{
  if(rpm_>20000)  return false;
  delete rpm;
  rpm=0;

  rpm=new unsigned int(rpm_);
  return true;
}

void OnVehicleData::reset_rpm(void)
{
  if(rpm)
    delete rpm;
  rpm=0;
}

bool OnVehicleData::set_fuelLevel(float fuelLevel_)
{
  if(fuelLevel_>100)  return false;
  if(fuelLevel_<0)  return false;
  delete fuelLevel;
  fuelLevel=0;

  fuelLevel=new float(fuelLevel_);
  return true;
}

void OnVehicleData::reset_fuelLevel(void)
{
  if(fuelLevel)
    delete fuelLevel;
  fuelLevel=0;
}

bool OnVehicleData::set_avgFuelEconomy(float avgFuelEconomy_)
{
  if(avgFuelEconomy_>7000)  return false;
  if(avgFuelEconomy_<0)  return false;
  delete avgFuelEconomy;
  avgFuelEconomy=0;

  avgFuelEconomy=new float(avgFuelEconomy_);
  return true;
}

void OnVehicleData::reset_avgFuelEconomy(void)
{
  if(avgFuelEconomy)
    delete avgFuelEconomy;
  avgFuelEconomy=0;
}

bool OnVehicleData::set_batteryVoltage(float batteryVoltage_)
{
  if(batteryVoltage_>26)  return false;
  if(batteryVoltage_<0)  return false;
  delete batteryVoltage;
  batteryVoltage=0;

  batteryVoltage=new float(batteryVoltage_);
  return true;
}

void OnVehicleData::reset_batteryVoltage(void)
{
  if(batteryVoltage)
    delete batteryVoltage;
  batteryVoltage=0;
}

bool OnVehicleData::set_externalTemperature(float externalTemperature_)
{
  if(externalTemperature_>100)  return false;
  if(externalTemperature_<-40)  return false;
  delete externalTemperature;
  externalTemperature=0;

  externalTemperature=new float(externalTemperature_);
  return true;
}

void OnVehicleData::reset_externalTemperature(void)
{
  if(externalTemperature)
    delete externalTemperature;
  externalTemperature=0;
}

bool OnVehicleData::set_vin(const std::string& vin_)
{
  if(vin_.length()>17)  return false;
  delete vin;
  vin=0;

  vin=new std::string(vin_);
  return true;
}

void OnVehicleData::reset_vin(void)
{
  if(vin)
    delete vin;
  vin=0;
}

bool OnVehicleData::set_prndl(const PRNDL& prndl_)
{
  if(!PRNDLMarshaller::checkIntegrityConst(prndl_))   return false;
  delete prndl;
  prndl=0;

  prndl=new PRNDL(prndl_);
  return true;
}

void OnVehicleData::reset_prndl(void)
{
  if(prndl)
    delete prndl;
  prndl=0;
}

bool OnVehicleData::set_tirePressure(const TireStatus& tirePressure_)
{
  if(!TireStatusMarshaller::checkIntegrityConst(tirePressure_))   return false;
  delete tirePressure;
  tirePressure=0;

  tirePressure=new TireStatus(tirePressure_);
  return true;
}

void OnVehicleData::reset_tirePressure(void)
{
  if(tirePressure)
    delete tirePressure;
  tirePressure=0;
}

bool OnVehicleData::set_batteryPackVoltage(float batteryPackVoltage_)
{
  if(batteryPackVoltage_>1000)  return false;
  if(batteryPackVoltage_<-1000)  return false;
  delete batteryPackVoltage;
  batteryPackVoltage=0;

  batteryPackVoltage=new float(batteryPackVoltage_);
  return true;
}

void OnVehicleData::reset_batteryPackVoltage(void)
{
  if(batteryPackVoltage)
    delete batteryPackVoltage;
  batteryPackVoltage=0;
}

bool OnVehicleData::set_batteryPackCurrent(float batteryPackCurrent_)
{
  if(batteryPackCurrent_>1000)  return false;
  if(batteryPackCurrent_<-1000)  return false;
  delete batteryPackCurrent;
  batteryPackCurrent=0;

  batteryPackCurrent=new float(batteryPackCurrent_);
  return true;
}

void OnVehicleData::reset_batteryPackCurrent(void)
{
  if(batteryPackCurrent)
    delete batteryPackCurrent;
  batteryPackCurrent=0;
}

bool OnVehicleData::set_batteryPackTemperature(int batteryPackTemperature_)
{
  if(batteryPackTemperature_>250)  return false;
  delete batteryPackTemperature;
  batteryPackTemperature=0;

  batteryPackTemperature=new int(batteryPackTemperature_);
  return true;
}

void OnVehicleData::reset_batteryPackTemperature(void)
{
  if(batteryPackTemperature)
    delete batteryPackTemperature;
  batteryPackTemperature=0;
}

bool OnVehicleData::set_engineTorque(int engineTorque_)
{
  if(engineTorque_>55000)  return false;
  delete engineTorque;
  engineTorque=0;

  engineTorque=new int(engineTorque_);
  return true;
}

void OnVehicleData::reset_engineTorque(void)
{
  if(engineTorque)
    delete engineTorque;
  engineTorque=0;
}

bool OnVehicleData::set_odometer(unsigned int odometer_)
{
  if(odometer_>2000000)  return false;
  delete odometer;
  odometer=0;

  odometer=new unsigned int(odometer_);
  return true;
}

void OnVehicleData::reset_odometer(void)
{
  if(odometer)
    delete odometer;
  odometer=0;
}

bool OnVehicleData::set_tripOdometer(unsigned int tripOdometer_)
{
  if(tripOdometer_>2000000)  return false;
  delete tripOdometer;
  tripOdometer=0;

  tripOdometer=new unsigned int(tripOdometer_);
  return true;
}

void OnVehicleData::reset_tripOdometer(void)
{
  if(tripOdometer)
    delete tripOdometer;
  tripOdometer=0;
}

bool OnVehicleData::set_genericbinary(const std::string& genericbinary_)
{
  if(genericbinary_.length()>500)  return false;
  delete genericbinary;
  genericbinary=0;

  genericbinary=new std::string(genericbinary_);
  return true;
}

void OnVehicleData::reset_genericbinary(void)
{
  if(genericbinary)
    delete genericbinary;
  genericbinary=0;
}




const GPSData* OnVehicleData::get_gps(void) const 
{
  return gps;
}

const float* OnVehicleData::get_speed(void) const 
{
  return speed;
}

const unsigned int* OnVehicleData::get_rpm(void) const 
{
  return rpm;
}

const float* OnVehicleData::get_fuelLevel(void) const 
{
  return fuelLevel;
}

const float* OnVehicleData::get_avgFuelEconomy(void) const 
{
  return avgFuelEconomy;
}

const float* OnVehicleData::get_batteryVoltage(void) const 
{
  return batteryVoltage;
}

const float* OnVehicleData::get_externalTemperature(void) const 
{
  return externalTemperature;
}

const std::string* OnVehicleData::get_vin(void) const 
{
  return vin;
}

const PRNDL* OnVehicleData::get_prndl(void) const 
{
  return prndl;
}

const TireStatus* OnVehicleData::get_tirePressure(void) const 
{
  return tirePressure;
}

const float* OnVehicleData::get_batteryPackVoltage(void) const 
{
  return batteryPackVoltage;
}

const float* OnVehicleData::get_batteryPackCurrent(void) const 
{
  return batteryPackCurrent;
}

const int* OnVehicleData::get_batteryPackTemperature(void) const 
{
  return batteryPackTemperature;
}

const int* OnVehicleData::get_engineTorque(void) const 
{
  return engineTorque;
}

const unsigned int* OnVehicleData::get_odometer(void) const 
{
  return odometer;
}

const unsigned int* OnVehicleData::get_tripOdometer(void) const 
{
  return tripOdometer;
}

const std::string* OnVehicleData::get_genericbinary(void) const 
{
  return genericbinary;
}

