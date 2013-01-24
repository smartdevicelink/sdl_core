#ifndef NSAPPLINKRPCV2_TIREPRESSURETELLTALE_INCLUDE
#define NSAPPLINKRPCV2_TIREPRESSURETELLTALE_INCLUDE


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

///  Reflects the status of the cluster instrument warning light.

  class TirePressureTellTale
  {
  public:
    enum TirePressureTellTaleInternal
    {
      INVALID_ENUM=-1,
      OFF=0,
      ON=1,
      FLASH=2
    };
  
    TirePressureTellTale() : mInternal(INVALID_ENUM)				{}
    TirePressureTellTale(TirePressureTellTaleInternal e) : mInternal(e)		{}
  
    TirePressureTellTaleInternal get(void) const	{ return mInternal; }
    void set(TirePressureTellTaleInternal e)		{ mInternal=e; }
  
  private:
    TirePressureTellTaleInternal mInternal;
    friend class TirePressureTellTaleMarshaller;
  };
  
}

#endif
