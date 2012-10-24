#ifndef COMPASSDIRECTION_INCLUDE
#define COMPASSDIRECTION_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


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

#endif
