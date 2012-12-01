#ifndef NSAPPLINKRPC_STARTTIME_V2_INCLUDE
#define NSAPPLINKRPC_STARTTIME_V2_INCLUDE




/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/


namespace NsAppLinkRPC
{

  class StartTime_v2
  {
  public:
  
    StartTime_v2(const StartTime_v2& c);
    StartTime_v2(void);
  
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

    friend class StartTime_v2Marshaller;


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
