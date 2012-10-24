#ifndef DRIVERDISTRACTIONSTATE_INCLUDE
#define DRIVERDISTRACTIONSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  Enumeration that describes possible states of driver distraction.

class DriverDistractionState
{
public:
  enum DriverDistractionStateInternal
  {
    INVALID_ENUM=-1,
    DD_ON=0,
    DD_OFF=1
  };

  DriverDistractionState() : mInternal(INVALID_ENUM)				{}
  DriverDistractionState(DriverDistractionStateInternal e) : mInternal(e)		{}

  DriverDistractionStateInternal get(void) const	{ return mInternal; }
  void set(DriverDistractionStateInternal e)		{ mInternal=e; }

private:
  DriverDistractionStateInternal mInternal;
  friend class DriverDistractionStateMarshaller;
};

#endif
