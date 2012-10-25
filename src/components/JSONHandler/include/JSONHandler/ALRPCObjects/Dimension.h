#ifndef DIMENSION_INCLUDE
#define DIMENSION_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
