#ifndef NSAPPLINKRPC_TBTSTATE_V2_INCLUDE
#define NSAPPLINKRPC_TBTSTATE_V2_INCLUDE


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

///  Enumeration that describes possible states of turn-by-turn client or AppLink app.

  class TBTState_v2
  {
  public:
    enum TBTState_v2Internal
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
  
    TBTState_v2() : mInternal(INVALID_ENUM)				{}
    TBTState_v2(TBTState_v2Internal e) : mInternal(e)		{}
  
    TBTState_v2Internal get(void) const	{ return mInternal; }
    void set(TBTState_v2Internal e)		{ mInternal=e; }
  
  private:
    TBTState_v2Internal mInternal;
    friend class TBTState_v2Marshaller;
  };
  
}

#endif
