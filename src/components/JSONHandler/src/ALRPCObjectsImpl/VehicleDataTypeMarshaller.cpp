#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/VehicleDataType.h"
#include "VehicleDataTypeMarshaller.h"
#include "VehicleDataTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const VehicleDataType::VehicleDataTypeInternal VehicleDataTypeMarshaller::getIndex(const char* s)
{
  if(!s)
    return VehicleDataType::INVALID_ENUM;
  const struct PerfectHashTable* p=VehicleDataType_intHash::getPointer(s,strlen(s));
  return p ? static_cast<VehicleDataType::VehicleDataTypeInternal>(p->idx) : VehicleDataType::INVALID_ENUM;
}


bool VehicleDataTypeMarshaller::fromJSON(const Json::Value& s,VehicleDataType& e)
{
  e.mInternal=VehicleDataType::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=VehicleDataType::INVALID_ENUM);
}


Json::Value VehicleDataTypeMarshaller::toJSON(const VehicleDataType& e)
{
  if(e.mInternal==VehicleDataType::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool VehicleDataTypeMarshaller::fromString(const std::string& s,VehicleDataType& e)
{
  e.mInternal=VehicleDataType::INVALID_ENUM;
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(fromJSON(json,e))  return true;
  }
  catch(...)
  {
    return false;
  }
  return false;
}

const std::string VehicleDataTypeMarshaller::toString(const VehicleDataType& e)
{
  Json::FastWriter writer;
  return e.mInternal==VehicleDataType::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable VehicleDataTypeMarshaller::mHashTable[16]=
{
  {"VEHICLEDATA_GPS",0},
  {"VEHICLEDATA_SPEED",1},
  {"VEHICLEDATA_FUELLEVEL",2},
  {"VEHICLEDATA_FUELECONOMY",3},
  {"VEHICLEDATA_ENGINERPM",4},
  {"VEHICLEDATA_BATTVOLTS",5},
  {"VEHICLEDATA_RAINSENSOR",6},
  {"VEHICLEDATA_ODOMETER",7},
  {"VEHICLEDATA_VIN",8},
  {"VEHICLEDATA_EXTERNTEMP",9},
  {"VEHICLEDATA_PRNDLSTATUS",10},
  {"VEHICLEDATA_TIREPRESSURE",11},
  {"VEHICLEDATA_BATTERYPACKVOLTAGE",12},
  {"VEHICLEDATA_BATTERYCURRENT",13},
  {"VEHICLEDATA_BATTERYTEMPERATURE",14},
  {"VEHICLEDATA_SATESN",15}
};
