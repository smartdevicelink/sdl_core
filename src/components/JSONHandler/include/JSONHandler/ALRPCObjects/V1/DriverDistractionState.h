#ifndef NSAPPLINKRPC_DRIVERDISTRACTIONSTATE_INCLUDE
#define NSAPPLINKRPC_DRIVERDISTRACTIONSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

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
  
}

#endif
