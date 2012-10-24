#ifndef VEHICLEDATATYPE_INCLUDE
#define VEHICLEDATATYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


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

#endif
