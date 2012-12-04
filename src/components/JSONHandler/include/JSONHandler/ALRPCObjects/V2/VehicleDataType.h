#ifndef NSAPPLINKRPCV2_VEHICLEDATATYPE_INCLUDE
#define NSAPPLINKRPCV2_VEHICLEDATATYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
