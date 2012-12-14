#ifndef NSAPPLINKRPCV2_ONVEHICLEDATA_INCLUDE
#define NSAPPLINKRPCV2_ONVEHICLEDATA_INCLUDE

#include <string>

#include "GPSData.h"
#include "PRNDL.h"
#include "TireStatus.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Callback for the periodic and non periodic vehicle data read function.

  class OnVehicleData : public NsAppLinkRPC::ALRPCMessage
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


/**
     See GPSData
     Subscribable
*/
      GPSData* gps;

/**
     The vehicle speed in kilometers per hour
     Subscribable
*/
      float* speed;	//!<  (-100,500)

/**
     The number of revolutions per minute of the engine
     Subscribable
*/
      unsigned int* rpm;	//!<  (0,20000)

///  The fuel level
      float* fuelLevel;	//!<  (0,100)

///  The average fuel economy in litres/100km
      float* avgFuelEconomy;	//!<  (0,7000)

///  The voltage in Volts
      float* batteryVoltage;	//!<  (0,26)

///  The external temperature in degrees celsius.
      float* externalTemperature;	//!<  (-40,100)

///  Vehicle identification number.
      std::string* vin;	//!< (17)

/**
     See PRNDL.
     Subscribable
*/
      PRNDL* prndl;

/**
     See TireStatus.
     Subscribable
*/
      TireStatus* tirePressure;

///  Battery pack voltage of hybrid and electrical vehicles in volts
      float* batteryPackVoltage;	//!<  (-1000,1000)

///  Battery current of hybrid and electrical vehicles in amperes
      float* batteryPackCurrent;	//!<  (-1000,1000)

///  Battery temperature of hybrid and electrical vehicles in degrees celsius
      int* batteryPackTemperature;	//!<  (-50,250)

/**
     Engine torque in Nm
     Subscribable
*/
      int* engineTorque;	//!<  (-10000,55000)

///  Odometer in km
      unsigned int* odometer;	//!<  (0,2000000)

///  Odometer of trip in km
      unsigned int* tripOdometer;	//!<  (0,2000000)

///  The hex value string is a string of hexadecimal chars, for example "FE12" or "1234ABCD".
      std::string* genericbinary;	//!< (500)
  };

}

#endif
