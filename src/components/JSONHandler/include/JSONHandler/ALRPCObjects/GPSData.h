#ifndef GPSDATA_INCLUDE
#define GPSDATA_INCLUDE


#include "CompassDirection.h"
#include "Dimension.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



///  Struct with the GPS data.

class GPSData
{
public:

  GPSData(const GPSData& c);
  GPSData(void);

  bool checkIntegrity(void);
// getters

  bool get_actual(void) const;
  int get_altitude(void) const;
  const CompassDirection& get_compassDirection(void) const;
  const Dimension& get_dimension(void) const;
  float get_hdop(void) const;
  float get_heading(void) const;
  float get_latitudeDegrees(void) const;
  float get_longitudeDegrees(void) const;
  float get_pdop(void) const;
  unsigned int get_satellites(void) const;
  unsigned int get_speed(void) const;
  unsigned int get_utcDay(void) const;
  unsigned int get_utcHours(void) const;
  unsigned int get_utcMinutes(void) const;
  unsigned int get_utcMonth(void) const;
  unsigned int get_utcSeconds(void) const;
  unsigned int get_utcYear(void) const;
  float get_vdop(void) const;

// setters

  bool set_actual(bool actual_);
  bool set_altitude(int altitude_);
  bool set_compassDirection(const CompassDirection& compassDirection_);
  bool set_dimension(const Dimension& dimension_);
  bool set_hdop(float hdop_);
  bool set_heading(float heading_);
  bool set_latitudeDegrees(float latitudeDegrees_);
  bool set_longitudeDegrees(float longitudeDegrees_);
  bool set_pdop(float pdop_);
  bool set_satellites(unsigned int satellites_);
  bool set_speed(unsigned int speed_);
  bool set_utcDay(unsigned int utcDay_);
  bool set_utcHours(unsigned int utcHours_);
  bool set_utcMinutes(unsigned int utcMinutes_);
  bool set_utcMonth(unsigned int utcMonth_);
  bool set_utcSeconds(unsigned int utcSeconds_);
  bool set_utcYear(unsigned int utcYear_);
  bool set_vdop(float vdop_);

private:

  friend class GPSDataMarshaller;

  bool actual;
  int altitude;	//!<  (-10000,10000)
  CompassDirection compassDirection;
  Dimension dimension;
  float hdop;	//!<  (0,31)
  float heading;	//!<  (0,360)
  float latitudeDegrees;
  float longitudeDegrees;
  float pdop;	//!<  (0,31)
  unsigned int satellites;	//!<  (0,31)
  unsigned int speed;	//!<  (0,400)
  unsigned int utcDay;	//!<  (1,31)
  unsigned int utcHours;	//!<  (0,23)
  unsigned int utcMinutes;	//!<  (0,59)
  unsigned int utcMonth;	//!<  (1,12)
  unsigned int utcSeconds;	//!<  (0,59)
  unsigned int utcYear;	//!<  (2010,2100)
  float vdop;	//!<  (0,31)
};

#endif
