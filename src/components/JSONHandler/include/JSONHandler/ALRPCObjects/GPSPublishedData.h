#ifndef GPSPUBLISHEDDATA_INCLUDE
#define GPSPUBLISHEDDATA_INCLUDE


#include "GPSData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



class GPSPublishedData
{
public:

  GPSPublishedData(const GPSPublishedData& c);
  GPSPublishedData(void);

  bool checkIntegrity(void);
// getters

  const GPSData& get_gpsData(void) const;
  bool get_gpsPositionValid(void) const;

// setters

  bool set_gpsData(const GPSData& gpsData_);
  bool set_gpsPositionValid(bool gpsPositionValid_);

private:

  friend class GPSPublishedDataMarshaller;

  GPSData gpsData;
  bool gpsPositionValid;
};

#endif
