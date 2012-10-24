#ifndef DIMENSION_INCLUDE
#define DIMENSION_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  The supported dimensions of the GPS

class Dimension
{
public:
  enum DimensionInternal
  {
    INVALID_ENUM=-1,

///  No GPS at all
    NO_FIX=0,

///  Longitude and lattitude
    FIX_2D=1,

///  Longitude and lattitude and altitude
    FIX_3D=2
  };

  Dimension() : mInternal(INVALID_ENUM)				{}
  Dimension(DimensionInternal e) : mInternal(e)		{}

  DimensionInternal get(void) const	{ return mInternal; }
  void set(DimensionInternal e)		{ mInternal=e; }

private:
  DimensionInternal mInternal;
  friend class DimensionMarshaller;
};

#endif
