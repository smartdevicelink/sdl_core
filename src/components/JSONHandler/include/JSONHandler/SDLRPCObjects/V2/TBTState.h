#ifndef NSAPPLINKRPCV2_TBTSTATE_INCLUDE
#define NSAPPLINKRPCV2_TBTSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Enumeration that describes possible states of turn-by-turn client or AppLink app.

  class TBTState
  {
  public:
    enum TBTStateInternal
    {
      INVALID_ENUM=-1,
      ROUTE_UPDATE_REQUEST=0,
      ROUTE_ACCEPTED=1,
      ROUTE_REFUSED=2,
      ROUTE_CANCELLED=3,
      ETA_REQUEST=4,
      NEXT_TURN_REQUEST=5,
      ROUTE_STATUS_REQUEST=6,
      ROUTE_SUMMARY_REQUEST=7,
      TRIP_STATUS_REQUEST=8,
      ROUTE_UPDATE_REQUEST_TIMEOUT=9
    };
  
    TBTState() : mInternal(INVALID_ENUM)				{}
    TBTState(TBTStateInternal e) : mInternal(e)		{}
  
    TBTStateInternal get(void) const	{ return mInternal; }
    void set(TBTStateInternal e)		{ mInternal=e; }
  
  private:
    TBTStateInternal mInternal;
    friend class TBTStateMarshaller;
  };
  
}

#endif
