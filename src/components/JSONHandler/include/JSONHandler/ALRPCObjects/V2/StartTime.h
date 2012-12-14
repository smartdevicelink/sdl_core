#ifndef NSAPPLINKRPCV2_STARTTIME_INCLUDE
#define NSAPPLINKRPCV2_STARTTIME_INCLUDE




/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/


namespace NsAppLinkRPCV2
{

  class StartTime
  {
  public:
  
    StartTime(const StartTime& c);
    StartTime(void);
  
    bool checkIntegrity(void);
  // getters

    unsigned int get_hours(void) const;
    unsigned int get_minutes(void) const;
    unsigned int get_seconds(void) const;

// setters

    bool set_hours(unsigned int hours_);
    bool set_minutes(unsigned int minutes_);
    bool set_seconds(unsigned int seconds_);

  private:

    friend class StartTimeMarshaller;


/**
     The hour of the media clock.
     Some radios only support a max of 19 hours. If out of range, it will be rejected.
*/
      unsigned int hours;	//!<  (0,59)
      unsigned int minutes;	//!<  (0,59)
      unsigned int seconds;	//!<  (0,59)
  };

}

#endif
