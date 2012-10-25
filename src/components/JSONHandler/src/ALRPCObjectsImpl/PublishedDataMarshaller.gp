%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name PublishedData_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
avgFuelEconomy,0
batteryPackCurrent,1
batteryPackTemperature,2
batteryPackVoltage,3
batteryVoltage,4
engineTorque,5
externalTemperature,6
fuelLevel,7
genericbinary,8
gps,9
odometer,10
prndl,11
rpm,12
speed,13
tirePressure,14
tripOdometer,15
vin,16
