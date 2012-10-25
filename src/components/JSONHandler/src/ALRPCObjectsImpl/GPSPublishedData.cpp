#include "../../include/JSONHandler/ALRPCObjects/GPSPublishedData.h"
#include "GPSPublishedDataMarshaller.h"
#include "GPSDataMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



GPSPublishedData::GPSPublishedData(const GPSPublishedData& c)
{
  *this=c;
}


bool GPSPublishedData::checkIntegrity(void)
{
  return GPSPublishedDataMarshaller::checkIntegrity(*this);
}


GPSPublishedData::GPSPublishedData(void)
{
}



bool GPSPublishedData::set_gpsData(const GPSData& gpsData_)
{
  if(!GPSDataMarshaller::checkIntegrityConst(gpsData_))   return false;
  gpsData=gpsData_;
  return true;
}

bool GPSPublishedData::set_gpsPositionValid(bool gpsPositionValid_)
{
  gpsPositionValid=gpsPositionValid_;
  return true;
}




const GPSData& GPSPublishedData::get_gpsData(void) const 
{
  return gpsData;
}


bool GPSPublishedData::get_gpsPositionValid(void) const
{
  return gpsPositionValid;
}


