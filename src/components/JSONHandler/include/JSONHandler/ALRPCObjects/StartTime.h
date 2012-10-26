#ifndef STARTTIME_INCLUDE
#define STARTTIME_INCLUDE




/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/



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

  unsigned int hours;	//!<  (0,59)
  unsigned int minutes;	//!<  (0,59)
  unsigned int seconds;	//!<  (0,59)
};

#endif
