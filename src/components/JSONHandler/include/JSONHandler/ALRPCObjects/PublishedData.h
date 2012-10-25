#ifndef PUBLISHEDDATA_INCLUDE
#define PUBLISHEDDATA_INCLUDE

#include <string>

#include "GPSPublishedData.h"
#include "PRNDL.h"
#include "TireStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



///  The content of PublishedData takes different forms based on the data type.  The data type is given in the subscriptionType in the OnDataPublished event message.
class PublishedData
{
public:

  enum eDiscriminator
  {
    INVALID_ENUM=-1,
    avgFuelEconomy=0,
    batteryPackCurrent=1,
    batteryPackTemperature=2,
    batteryPackVoltage=3,
    batteryVoltage=4,
    engineTorque=5,
    externalTemperature=6,
    fuelLevel=7,
    genericbinary=8,
    gps=9,
    odometer=10,
    prndl=11,
    rpm=12,
    speed=13,
    tirePressure=14,
    tripOdometer=15,
    vin=16
  };

  struct tUnion
  {
    eDiscriminator mDiscriminator;
    float avgFuelEconomy;	//!<  (0,7000)
    float batteryPackCurrent;	//!<  (-1000,1000)
    int batteryPackTemperature;	//!<  (-50,250)
    float batteryPackVoltage;	//!<  (-1000,1000)
    float batteryVoltage;	//!<  (0,26)
    int engineTorque;	//!<  (-10000,55000)
    float externalTemperature;	//!<  (-40,100)
    float fuelLevel;	//!<  (0,100)
    std::string genericbinary;
    GPSPublishedData gps;
    unsigned int odometer;	//!<  (0,2000000)
    PRNDL prndl;
    unsigned int rpm;	//!<  (0,20000)
    float speed;	//!<  (-100,500)
    TireStatus tirePressure;
    unsigned int tripOdometer;	//!<  (0,2000000)
    std::string vin;	//!< (17)
  };

  PublishedData(const PublishedData& c);
  PublishedData(void);

  ~PublishedData(void);

//! return true if all constraints valid
  bool checkIntegrity(void);

// getters
  eDiscriminator getType(void);
  eDiscriminator get_All(tUnion& t);
//setters

  bool set_avgFuelEconomy(float avgFuelEconomy_);
  bool set_batteryPackCurrent(float batteryPackCurrent_);
  bool set_batteryPackTemperature(int batteryPackTemperature_);
  bool set_batteryPackVoltage(float batteryPackVoltage_);
  bool set_batteryVoltage(float batteryVoltage_);
  bool set_engineTorque(int engineTorque_);
  bool set_externalTemperature(float externalTemperature_);
  bool set_fuelLevel(float fuelLevel_);
  bool set_genericbinary(const std::string& genericbinary_);
  bool set_gps(const GPSPublishedData& gps_);
  bool set_odometer(unsigned int odometer_);
  bool set_prndl(const PRNDL& prndl_);
  bool set_rpm(unsigned int rpm_);
  bool set_speed(float speed_);
  bool set_tirePressure(const TireStatus& tirePressure_);
  bool set_tripOdometer(unsigned int tripOdometer_);
  bool set_vin(const std::string& vin_);

  bool set_All(const tUnion& t);

private:
  tUnion mInternal;

  friend class PublishedDataMarshaller;
};

#endif
