#ifndef TBTSTATE_INCLUDE
#define TBTSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


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

#endif
