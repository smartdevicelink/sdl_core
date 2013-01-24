#ifndef NSAPPLINKRPCV2_COMPASSDIRECTION_INCLUDE
#define NSAPPLINKRPCV2_COMPASSDIRECTION_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
