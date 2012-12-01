#ifndef NSAPPLINKRPC_VEHICLEDATATYPE_INCLUDE
#define NSAPPLINKRPC_VEHICLEDATATYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Defines the data types that can be published and subscribed to.

  class VehicleDataType
  {
  public:
    enum VehicleDataTypeInternal
    {
      INVALID_ENUM=-1,

/**
     Notifies GPSData
     may be subscribed
*/
      VEHICLEDATA_GPS=0,
      VEHICLEDATA_SPEED=1,
      VEHICLEDATA_FUELLEVEL=2,
      VEHICLEDATA_FUELECONOMY=3,
      VEHICLEDATA_ENGINERPM=4,
      VEHICLEDATA_BATTVOLTS=5,
      VEHICLEDATA_RAINSENSOR=6,
      VEHICLEDATA_ODOMETER=7,
      VEHICLEDATA_VIN=8,
      VEHICLEDATA_EXTERNTEMP=9,
      VEHICLEDATA_PRNDLSTATUS=10,
      VEHICLEDATA_TIREPRESSURE=11,

///  Battery pack voltage of hybrid and electrical vehicles
      VEHICLEDATA_BATTERYPACKVOLTAGE=12,

///  Battery current of hybrid and electrical vehicles
      VEHICLEDATA_BATTERYCURRENT=13,

///  Battery temperature of hybrid and electrical vehicles
      VEHICLEDATA_BATTERYTEMPERATURE=14,

///  Satellite radio serial number (ESN)
      VEHICLEDATA_SATESN=15
    };
  
    VehicleDataType() : mInternal(INVALID_ENUM)				{}
    VehicleDataType(VehicleDataTypeInternal e) : mInternal(e)		{}
  
    VehicleDataTypeInternal get(void) const	{ return mInternal; }
    void set(VehicleDataTypeInternal e)		{ mInternal=e; }
  
  private:
    VehicleDataTypeInternal mInternal;
    friend class VehicleDataTypeMarshaller;
  };
  
}

#endif
