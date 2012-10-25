#ifndef TBTSTATE_INCLUDE
#define TBTSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


///  Enumeration that describes possible states of turn-by-turn module.

class TBTState
{
public:
  enum TBTStateInternal
  {
    INVALID_ENUM=-1,
    ROUTE_UPDATE_REQUEST=0,
    ROUTE_ACCEPTED=1,
    ROUTE_REFUSED=2,
    ROUTE_CANCELLED=3
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
