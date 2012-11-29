#ifndef NSAPPLINKRPC_COMPASSDIRECTION_INCLUDE
#define NSAPPLINKRPC_COMPASSDIRECTION_INCLUDE


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

///  The list of potential compass directions

  class CompassDirection
  {
  public:
    enum CompassDirectionInternal
    {
      INVALID_ENUM=-1,
      NORTH=0,
      NORTHWEST=1,
      WEST=2,
      SOUTHWEST=3,
      SOUTH=4,
      SOUTHEAST=5,
      EAST=6,
      NORTHEAST=7
    };
  
    CompassDirection() : mInternal(INVALID_ENUM)				{}
    CompassDirection(CompassDirectionInternal e) : mInternal(e)		{}
  
    CompassDirectionInternal get(void) const	{ return mInternal; }
    void set(CompassDirectionInternal e)		{ mInternal=e; }
  
  private:
    CompassDirectionInternal mInternal;
    friend class CompassDirectionMarshaller;
  };
  
}

#endif
