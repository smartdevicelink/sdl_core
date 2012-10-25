#ifndef DRIVERDISTRACTIONSTATE_INCLUDE
#define DRIVERDISTRACTIONSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
