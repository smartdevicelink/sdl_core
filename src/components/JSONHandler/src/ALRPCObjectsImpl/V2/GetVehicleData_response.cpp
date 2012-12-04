#include "../include/JSONHandler/ALRPCObjects/V2/GetVehicleData_response.h"
#include "GetVehicleData_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "GPSDataMarshaller.h"
#include "PRNDLMarshaller.h"
#include "ResultMarshaller.h"
#include "TireStatusMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
GetVehicleData_response& GetVehicleData_response::operator =(const GetVehicleData_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;
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
  satRadioESN= c.satRadioESN ? new std::string(c.satRadioESN[0]) : 0;

  return *this;
}


GetVehicleData_response::~GetVehicleData_response(void)
{
  if(info)
    delete info;
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
  if(satRadioESN)
    delete satRadioESN;
}


GetVehicleData_response::GetVehicleData_response(const GetVehicleData_response& c) : ALRPC2Message(c)
{
  *this=c;
}


bool GetVehicleData_response::checkIntegrity(void)
{
  return GetVehicleData_responseMarshaller::checkIntegrity(*this);
}


GetVehicleData_response::GetVehicleData_response(void) : ALRPC2Message(PROTOCOL_VERSION),
      info(0),
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
    satRadioESN(0)
{
}



bool GetVehicleData_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool GetVehicleData_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool GetVehicleData_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void GetVehicleData_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}

bool GetVehicleData_response::set_gps(const GPSData& gps_)
{
  if(!GPSDataMarshaller::checkIntegrityConst(gps_))   return false;
  delete gps;
  gps=0;

  gps=new GPSData(gps_);
  return true;
}

void GetVehicleData_response::reset_gps(void)
{
  if(gps)
    delete gps;
  gps=0;
}

bool GetVehicleData_response::set_speed(float speed_)
{
  if(speed_>500)  return false;
  if(speed_<-100)  return false;
  delete speed;
  speed=0;

  speed=new float(speed_);
  return true;
}

void GetVehicleData_response::reset_speed(void)
{
  if(speed)
    delete speed;
  speed=0;
}

bool GetVehicleData_response::set_rpm(unsigned int rpm_)
{
  if(rpm_>20000)  return false;
  delete rpm;
  rpm=0;

  rpm=new unsigned int(rpm_);
  return true;
}

void GetVehicleData_response::reset_rpm(void)
{
  if(rpm)
    delete rpm;
  rpm=0;
}

bool GetVehicleData_response::set_fuelLevel(float fuelLevel_)
{
  if(fuelLevel_>100)  return false;
  if(fuelLevel_<0)  return false;
  delete fuelLevel;
  fuelLevel=0;

  fuelLevel=new float(fuelLevel_);
  return true;
}

void GetVehicleData_response::reset_fuelLevel(void)
{
  if(fuelLevel)
    delete fuelLevel;
  fuelLevel=0;
}

bool GetVehicleData_response::set_avgFuelEconomy(float avgFuelEconomy_)
{
  if(avgFuelEconomy_>7000)  return false;
  if(avgFuelEconomy_<0)  return false;
  delete avgFuelEconomy;
  avgFuelEconomy=0;

  avgFuelEconomy=new float(avgFuelEconomy_);
  return true;
}

void GetVehicleData_response::reset_avgFuelEconomy(void)
{
  if(avgFuelEconomy)
    delete avgFuelEconomy;
  avgFuelEconomy=0;
}

bool GetVehicleData_response::set_batteryVoltage(float batteryVoltage_)
{
  if(batteryVoltage_>26)  return false;
  if(batteryVoltage_<0)  return false;
  delete batteryVoltage;
  batteryVoltage=0;

  batteryVoltage=new float(batteryVoltage_);
  return true;
}

void GetVehicleData_response::reset_batteryVoltage(void)
{
  if(batteryVoltage)
    delete batteryVoltage;
  batteryVoltage=0;
}

bool GetVehicleData_response::set_externalTemperature(float externalTemperature_)
{
  if(externalTemperature_>100)  return false;
  if(externalTemperature_<-40)  return false;
  delete externalTemperature;
  externalTemperature=0;

  externalTemperature=new float(externalTemperature_);
  return true;
}

void GetVehicleData_response::reset_externalTemperature(void)
{
  if(externalTemperature)
    delete externalTemperature;
  externalTemperature=0;
}

bool GetVehicleData_response::set_vin(const std::string& vin_)
{
  if(vin_.length()>17)  return false;
  delete vin;
  vin=0;

  vin=new std::string(vin_);
  return true;
}

void GetVehicleData_response::reset_vin(void)
{
  if(vin)
    delete vin;
  vin=0;
}

bool GetVehicleData_response::set_prndl(const PRNDL& prndl_)
{
  if(!PRNDLMarshaller::checkIntegrityConst(prndl_))   return false;
  delete prndl;
  prndl=0;

  prndl=new PRNDL(prndl_);
  return true;
}

void GetVehicleData_response::reset_prndl(void)
{
  if(prndl)
    delete prndl;
  prndl=0;
}

bool GetVehicleData_response::set_tirePressure(const TireStatus& tirePressure_)
{
  if(!TireStatusMarshaller::checkIntegrityConst(tirePressure_))   return false;
  delete tirePressure;
  tirePressure=0;

  tirePressure=new TireStatus(tirePressure_);
  return true;
}

void GetVehicleData_response::reset_tirePressure(void)
{
  if(tirePressure)
    delete tirePressure;
  tirePressure=0;
}

bool GetVehicleData_response::set_batteryPackVoltage(float batteryPackVoltage_)
{
  if(batteryPackVoltage_>1000)  return false;
  if(batteryPackVoltage_<-1000)  return false;
  delete batteryPackVoltage;
  batteryPackVoltage=0;

  batteryPackVoltage=new float(batteryPackVoltage_);
  return true;
}

void GetVehicleData_response::reset_batteryPackVoltage(void)
{
  if(batteryPackVoltage)
    delete batteryPackVoltage;
  batteryPackVoltage=0;
}

bool GetVehicleData_response::set_batteryPackCurrent(float batteryPackCurrent_)
{
  if(batteryPackCurrent_>1000)  return false;
  if(batteryPackCurrent_<-1000)  return false;
  delete batteryPackCurrent;
  batteryPackCurrent=0;

  batteryPackCurrent=new float(batteryPackCurrent_);
  return true;
}

void GetVehicleData_response::reset_batteryPackCurrent(void)
{
  if(batteryPackCurrent)
    delete batteryPackCurrent;
  batteryPackCurrent=0;
}

bool GetVehicleData_response::set_batteryPackTemperature(int batteryPackTemperature_)
{
  if(batteryPackTemperature_>250)  return false;
  delete batteryPackTemperature;
  batteryPackTemperature=0;

  batteryPackTemperature=new int(batteryPackTemperature_);
  return true;
}

void GetVehicleData_response::reset_batteryPackTemperature(void)
{
  if(batteryPackTemperature)
    delete batteryPackTemperature;
  batteryPackTemperature=0;
}

bool GetVehicleData_response::set_engineTorque(int engineTorque_)
{
  if(engineTorque_>55000)  return false;
  delete engineTorque;
  engineTorque=0;

  engineTorque=new int(engineTorque_);
  return true;
}

void GetVehicleData_response::reset_engineTorque(void)
{
  if(engineTorque)
    delete engineTorque;
  engineTorque=0;
}

bool GetVehicleData_response::set_odometer(unsigned int odometer_)
{
  if(odometer_>2000000)  return false;
  delete odometer;
  odometer=0;

  odometer=new unsigned int(odometer_);
  return true;
}

void GetVehicleData_response::reset_odometer(void)
{
  if(odometer)
    delete odometer;
  odometer=0;
}

bool GetVehicleData_response::set_tripOdometer(unsigned int tripOdometer_)
{
  if(tripOdometer_>2000000)  return false;
  delete tripOdometer;
  tripOdometer=0;

  tripOdometer=new unsigned int(tripOdometer_);
  return true;
}

void GetVehicleData_response::reset_tripOdometer(void)
{
  if(tripOdometer)
    delete tripOdometer;
  tripOdometer=0;
}

bool GetVehicleData_response::set_satRadioESN(const std::string& satRadioESN_)
{
  if(satRadioESN_.length()>500)  return false;
  delete satRadioESN;
  satRadioESN=0;

  satRadioESN=new std::string(satRadioESN_);
  return true;
}

void GetVehicleData_response::reset_satRadioESN(void)
{
  if(satRadioESN)
    delete satRadioESN;
  satRadioESN=0;
}




bool GetVehicleData_response::get_success(void) const
{
  return success;
}

const Result& GetVehicleData_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* GetVehicleData_response::get_info(void) const 
{
  return info;
}

const GPSData* GetVehicleData_response::get_gps(void) const 
{
  return gps;
}

const float* GetVehicleData_response::get_speed(void) const 
{
  return speed;
}

const unsigned int* GetVehicleData_response::get_rpm(void) const 
{
  return rpm;
}

const float* GetVehicleData_response::get_fuelLevel(void) const 
{
  return fuelLevel;
}

const float* GetVehicleData_response::get_avgFuelEconomy(void) const 
{
  return avgFuelEconomy;
}

const float* GetVehicleData_response::get_batteryVoltage(void) const 
{
  return batteryVoltage;
}

const float* GetVehicleData_response::get_externalTemperature(void) const 
{
  return externalTemperature;
}

const std::string* GetVehicleData_response::get_vin(void) const 
{
  return vin;
}

const PRNDL* GetVehicleData_response::get_prndl(void) const 
{
  return prndl;
}

const TireStatus* GetVehicleData_response::get_tirePressure(void) const 
{
  return tirePressure;
}

const float* GetVehicleData_response::get_batteryPackVoltage(void) const 
{
  return batteryPackVoltage;
}

const float* GetVehicleData_response::get_batteryPackCurrent(void) const 
{
  return batteryPackCurrent;
}

const int* GetVehicleData_response::get_batteryPackTemperature(void) const 
{
  return batteryPackTemperature;
}

const int* GetVehicleData_response::get_engineTorque(void) const 
{
  return engineTorque;
}

const unsigned int* GetVehicleData_response::get_odometer(void) const 
{
  return odometer;
}

const unsigned int* GetVehicleData_response::get_tripOdometer(void) const 
{
  return tripOdometer;
}

const std::string* GetVehicleData_response::get_satRadioESN(void) const 
{
  return satRadioESN;
}

