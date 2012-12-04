#ifndef NSAPPLINKRPCV2_DIMENSION_INCLUDE
#define NSAPPLINKRPCV2_DIMENSION_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

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
  
}

#endif
