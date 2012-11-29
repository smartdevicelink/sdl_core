#ifndef NSAPPLINKRPC_DIMENSION_INCLUDE
#define NSAPPLINKRPC_DIMENSION_INCLUDE


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
