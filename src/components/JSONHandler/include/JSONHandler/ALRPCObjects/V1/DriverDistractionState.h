#ifndef NSAPPLINKRPC_DRIVERDISTRACTIONSTATE_INCLUDE
#define NSAPPLINKRPC_DRIVERDISTRACTIONSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
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
