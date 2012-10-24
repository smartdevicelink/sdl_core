#ifndef ONVEHICLEDATA_INCLUDE
#define ONVEHICLEDATA_INCLUDE

#include <string>

#include "GPSData.h"
#include "PRNDL.h"
#include "TireStatus.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Callback for the periodic and non periodic vehicle data read function.

class OnVehicleData : public ALRPCNotification
{
public:

  OnVehicleData(const OnVehicleData& c);
  OnVehicleData(void);
  
  virtual ~OnVehicleData(void);

  OnVehicleData& operator =(const OnVehicleData&);

  bool checkIntegrity(void);

  const GPSData* get_gps(void) const;
  const float* get_speed(void) const;
  const unsigned int* get_rpm(void) const;
  const float* get_fuelLevel(void) const;
  const float* get_avgFuelEconomy(void) const;
  const float* get_batteryVoltage(void) const;
  const float* get_externalTemperature(void) const;
  const std::string* get_vin(void) const;
  const PRNDL* get_prndl(void) const;
  const TireStatus* get_tirePressure(void) const;
  const float* get_batteryPackVoltage(void) const;
  const float* get_batteryPackCurrent(void) const;
  const int* get_batteryPackTemperature(void) const;
  const int* get_engineTorque(void) const;
  const unsigned int* get_odometer(void) const;
  const unsigned int* get_tripOdometer(void) const;
  const std::string* get_genericbinary(void) const;

  void reset_gps(void);
  bool set_gps(const GPSData& gps_);
  void reset_speed(void);
  bool set_speed(float speed_);
  void reset_rpm(void);
  bool set_rpm(unsigned int rpm_);
  void reset_fuelLevel(void);
  bool set_fuelLevel(float fuelLevel_);
  void reset_avgFuelEconomy(void);
  bool set_avgFuelEconomy(float avgFuelEconomy_);
  void reset_batteryVoltage(void);
  bool set_batteryVoltage(float batteryVoltage_);
  void reset_externalTemperature(void);
  bool set_externalTemperature(float externalTemperature_);
  void reset_vin(void);
  bool set_vin(const std::string& vin_);
  void reset_prndl(void);
  bool set_prndl(const PRNDL& prndl_);
  void reset_tirePressure(void);
  bool set_tirePressure(const TireStatus& tirePressure_);
  void reset_batteryPackVoltage(void);
  bool set_batteryPackVoltage(float batteryPackVoltage_);
  void reset_batteryPackCurrent(void);
  bool set_batteryPackCurrent(float batteryPackCurrent_);
  void reset_batteryPackTemperature(void);
  bool set_batteryPackTemperature(int batteryPackTemperature_);
  void reset_engineTorque(void);
  bool set_engineTorque(int engineTorque_);
  void reset_odometer(void);
  bool set_odometer(unsigned int odometer_);
  void reset_tripOdometer(void);
  bool set_tripOdometer(unsigned int tripOdometer_);
  void reset_genericbinary(void);
  bool set_genericbinary(const std::string& genericbinary_);

private:

  friend class OnVehicleDataMarshaller;

  GPSData* gps;
  float* speed;	//!<  (-100,500)
  unsigned int* rpm;	//!<  (0,20000)
  float* fuelLevel;	//!<  (0,100)
  float* avgFuelEconomy;	//!<  (0,7000)
  float* batteryVoltage;	//!<  (0,26)
  float* externalTemperature;	//!<  (-40,100)
  std::string* vin;	//!< (17)
  PRNDL* prndl;
  TireStatus* tirePressure;
  float* batteryPackVoltage;	//!<  (-1000,1000)
  float* batteryPackCurrent;	//!<  (-1000,1000)
  int* batteryPackTemperature;	//!<  (-50,250)
  int* engineTorque;	//!<  (-10000,55000)
  unsigned int* odometer;	//!<  (0,2000000)
  unsigned int* tripOdometer;	//!<  (0,2000000)
  std::string* genericbinary;	//!< (500)
};

#endif
