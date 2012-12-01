#ifndef NSAPPLINKRPC_TIREPRESSURETELLTALE_INCLUDE
#define NSAPPLINKRPC_TIREPRESSURETELLTALE_INCLUDE


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
