//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_GPSDATA_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_GPSDATA_INCLUDE


#include "CompassDirection.h"
#include "Dimension.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPCV2
{

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


/**
     True, if actual.
     False, if infered.
*/
      bool actual;

///  Altitude in meters
      int altitude;	//!<  (-10000,10000)

///  See CompassDirection.
      CompassDirection compassDirection;

///  See Dimension
      Dimension dimension;

///  HDOP.
      unsigned int hdop;	//!<  (0,31)

///  The heading. North is 0
      unsigned int heading;	//!<  (0,360)
      int latitudeDegrees;	//!<  (-1000000000,1000000000)
      int longitudeDegrees;	//!<  (-1000000000,1000000000)

///  PDOP.
      unsigned int pdop;	//!<  (0,31)

///  Number of satellites in view
      unsigned int satellites;	//!<  (0,31)

///  The speed in KPH
      unsigned int speed;	//!<  (0,400)

///  The current UTC day.
      unsigned int utcDay;	//!<  (1,31)

///  The current UTC hour.
      unsigned int utcHours;	//!<  (0,23)

///  The current UTC minute.
      unsigned int utcMinutes;	//!<  (0,59)

///  The current UTC month.
      unsigned int utcMonth;	//!<  (1,12)

///  The current UTC second.
      unsigned int utcSeconds;	//!<  (0,59)

///  The current UTC year.
      unsigned int utcYear;	//!<  (2010,2100)

///  VDOP.
      unsigned int vdop;	//!<  (0,31)
  };

}

#endif
