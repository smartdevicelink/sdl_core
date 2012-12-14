#ifndef NSAPPLINKRPCV2_SINGLETIREPRESSURESTATUS_INCLUDE
#define NSAPPLINKRPCV2_SINGLETIREPRESSURESTATUS_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
