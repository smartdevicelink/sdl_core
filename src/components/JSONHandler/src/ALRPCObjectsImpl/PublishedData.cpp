#include <cstring>

#include "../../include/JSONHandler/ALRPCObjects/PublishedData.h"
#include "GPSPublishedDataMarshaller.h"
#include "PRNDLMarshaller.h"
#include "TireStatusMarshaller.h"
#include "PublishedDataMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


PublishedData::~PublishedData(void)
{
}

PublishedData::PublishedData(const PublishedData& c)
{
  *this=c;
}

PublishedData::PublishedData(void)
{
  mInternal.mDiscriminator=INVALID_ENUM;
}


bool PublishedData::checkIntegrity(void)
{
  return PublishedDataMarshaller::checkIntegrity(*this); 
}

PublishedData::eDiscriminator PublishedData::getType(void)
{
  return mInternal.mDiscriminator;
}

PublishedData::eDiscriminator PublishedData::get_All(tUnion& t)
{
  t=mInternal;
  return mInternal.mDiscriminator;
}

bool PublishedData::set_All(const tUnion& t)
{
  mInternal=t;
  return checkIntegrity();
}
bool PublishedData::set_avgFuelEconomy(float avgFuelEconomy_)
{
  if(avgFuelEconomy_>7000)  return false;
  if(avgFuelEconomy_<0)  return false;
  mInternal.mDiscriminator=avgFuelEconomy;
  mInternal.avgFuelEconomy=avgFuelEconomy_;
  return true;
}

bool PublishedData::set_batteryPackCurrent(float batteryPackCurrent_)
{
  if(batteryPackCurrent_>1000)  return false;
  if(batteryPackCurrent_<-1000)  return false;
  mInternal.mDiscriminator=batteryPackCurrent;
  mInternal.batteryPackCurrent=batteryPackCurrent_;
  return true;
}

bool PublishedData::set_batteryPackTemperature(int batteryPackTemperature_)
{
  if(batteryPackTemperature_>250)  return false;
  mInternal.mDiscriminator=batteryPackTemperature;
  mInternal.batteryPackTemperature=batteryPackTemperature_;
  return true;
}

bool PublishedData::set_batteryPackVoltage(float batteryPackVoltage_)
{
  if(batteryPackVoltage_>1000)  return false;
  if(batteryPackVoltage_<-1000)  return false;
  mInternal.mDiscriminator=batteryPackVoltage;
  mInternal.batteryPackVoltage=batteryPackVoltage_;
  return true;
}

bool PublishedData::set_batteryVoltage(float batteryVoltage_)
{
  if(batteryVoltage_>26)  return false;
  if(batteryVoltage_<0)  return false;
  mInternal.mDiscriminator=batteryVoltage;
  mInternal.batteryVoltage=batteryVoltage_;
  return true;
}

bool PublishedData::set_engineTorque(int engineTorque_)
{
  if(engineTorque_>55000)  return false;
  mInternal.mDiscriminator=engineTorque;
  mInternal.engineTorque=engineTorque_;
  return true;
}

bool PublishedData::set_externalTemperature(float externalTemperature_)
{
  if(externalTemperature_>100)  return false;
  if(externalTemperature_<-40)  return false;
  mInternal.mDiscriminator=externalTemperature;
  mInternal.externalTemperature=externalTemperature_;
  return true;
}

bool PublishedData::set_fuelLevel(float fuelLevel_)
{
  if(fuelLevel_>100)  return false;
  if(fuelLevel_<0)  return false;
  mInternal.mDiscriminator=fuelLevel;
  mInternal.fuelLevel=fuelLevel_;
  return true;
}

bool PublishedData::set_genericbinary(const std::string& genericbinary_)
{
  mInternal.mDiscriminator=genericbinary;
  mInternal.genericbinary=genericbinary_;
  return true;
}

bool PublishedData::set_gps(const GPSPublishedData& gps_)
{
  if(!GPSPublishedDataMarshaller::checkIntegrityConst(gps_))   return false;
  mInternal.mDiscriminator=gps;
  mInternal.gps=gps_;
  return true;
}

bool PublishedData::set_odometer(unsigned int odometer_)
{
  if(odometer_>2000000)  return false;
  mInternal.mDiscriminator=odometer;
  mInternal.odometer=odometer_;
  return true;
}

bool PublishedData::set_prndl(const PRNDL& prndl_)
{
  if(!PRNDLMarshaller::checkIntegrityConst(prndl_))   return false;
  mInternal.mDiscriminator=prndl;
  mInternal.prndl=prndl_;
  return true;
}

bool PublishedData::set_rpm(unsigned int rpm_)
{
  if(rpm_>20000)  return false;
  mInternal.mDiscriminator=rpm;
  mInternal.rpm=rpm_;
  return true;
}

bool PublishedData::set_speed(float speed_)
{
  if(speed_>500)  return false;
  if(speed_<-100)  return false;
  mInternal.mDiscriminator=speed;
  mInternal.speed=speed_;
  return true;
}

bool PublishedData::set_tirePressure(const TireStatus& tirePressure_)
{
  if(!TireStatusMarshaller::checkIntegrityConst(tirePressure_))   return false;
  mInternal.mDiscriminator=tirePressure;
  mInternal.tirePressure=tirePressure_;
  return true;
}

bool PublishedData::set_tripOdometer(unsigned int tripOdometer_)
{
  if(tripOdometer_>2000000)  return false;
  mInternal.mDiscriminator=tripOdometer;
  mInternal.tripOdometer=tripOdometer_;
  return true;
}

bool PublishedData::set_vin(const std::string& vin_)
{
  if(vin_.length()>17)  return false;
  mInternal.mDiscriminator=vin;
  mInternal.vin=vin_;
  return true;
}

