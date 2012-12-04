#ifndef NSAPPLINKRPCV2_TIREPRESSURETELLTALE_INCLUDE
#define NSAPPLINKRPCV2_TIREPRESSURETELLTALE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
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
