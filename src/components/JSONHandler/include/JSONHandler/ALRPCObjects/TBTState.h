#ifndef APPLINKRPC_TBTSTATE_INCLUDE
#define APPLINKRPC_TBTSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
{

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
  
}

#endif
