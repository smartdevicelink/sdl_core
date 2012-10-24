#ifndef GPSDATA_INCLUDE
#define GPSDATA_INCLUDE


#include "CompassDirection.h"
#include "Dimension.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
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
  unsigned int get_hdop(void) const;
  unsigned int get_heading(void) const;
  int get_latitudeDegrees(void) const;
  int get_longitudeDegrees(void) const;
  unsigned int get_pdop(void) const;
  unsigned int get_satellites(void) const;
  unsigned int get_speed(void) const;
  unsigned int get_utcDay(void) const;
  unsigned int get_utcHours(void) const;
  unsigned int get_utcMinutes(void) const;
  unsigned int get_utcMonth(void) const;
  unsigned int get_utcSeconds(void) const;
  unsigned int get_utcYear(void) const;
  unsigned int get_vdop(void) const;

// setters

  bool set_actual(bool actual_);
  bool set_altitude(int altitude_);
  bool set_compassDirection(const CompassDirection& compassDirection_);
  bool set_dimension(const Dimension& dimension_);
  bool set_hdop(unsigned int hdop_);
  bool set_heading(unsigned int heading_);
  bool set_latitudeDegrees(int latitudeDegrees_);
  bool set_longitudeDegrees(int longitudeDegrees_);
  bool set_pdop(unsigned int pdop_);
  bool set_satellites(unsigned int satellites_);
  bool set_speed(unsigned int speed_);
  bool set_utcDay(unsigned int utcDay_);
  bool set_utcHours(unsigned int utcHours_);
  bool set_utcMinutes(unsigned int utcMinutes_);
  bool set_utcMonth(unsigned int utcMonth_);
  bool set_utcSeconds(unsigned int utcSeconds_);
  bool set_utcYear(unsigned int utcYear_);
  bool set_vdop(unsigned int vdop_);

private:

  friend class GPSDataMarshaller;

  bool actual;
  int altitude;	//!<  (-10000,10000)
  CompassDirection compassDirection;
  Dimension dimension;
  unsigned int hdop;	//!<  (0,31)
  unsigned int heading;	//!<  (0,360)
  int latitudeDegrees;	//!<  (-1000000000,1000000000)
  int longitudeDegrees;	//!<  (-1000000000,1000000000)
  unsigned int pdop;	//!<  (0,31)
  unsigned int satellites;	//!<  (0,31)
  unsigned int speed;	//!<  (0,400)
  unsigned int utcDay;	//!<  (1,31)
  unsigned int utcHours;	//!<  (0,23)
  unsigned int utcMinutes;	//!<  (0,59)
  unsigned int utcMonth;	//!<  (1,12)
  unsigned int utcSeconds;	//!<  (0,59)
  unsigned int utcYear;	//!<  (2010,2100)
  unsigned int vdop;	//!<  (0,31)
};

#endif
