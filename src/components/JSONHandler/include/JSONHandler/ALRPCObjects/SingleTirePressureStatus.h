#ifndef NSAPPLINKRPC_SINGLETIREPRESSURESTATUS_INCLUDE
#define NSAPPLINKRPC_SINGLETIREPRESSURESTATUS_INCLUDE


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

///  The pressure status of a tire.

  class SingleTirePressureStatus
  {
  public:
    enum SingleTirePressureStatusInternal
    {
      INVALID_ENUM=-1,
      UNKNOWN=0,
      NORMAL=1,
      LOW=2,
      FAULT=3,
      NOT_SUPPORTED=4
    };
  
    SingleTirePressureStatus() : mInternal(INVALID_ENUM)				{}
    SingleTirePressureStatus(SingleTirePressureStatusInternal e) : mInternal(e)		{}
  
    SingleTirePressureStatusInternal get(void) const	{ return mInternal; }
    void set(SingleTirePressureStatusInternal e)		{ mInternal=e; }
  
  private:
    SingleTirePressureStatusInternal mInternal;
    friend class SingleTirePressureStatusMarshaller;
  };
  
}

#endif
