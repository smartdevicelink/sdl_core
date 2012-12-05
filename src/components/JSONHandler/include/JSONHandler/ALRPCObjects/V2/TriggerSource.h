#ifndef NSAPPLINKRPCV2_TRIGGERSOURCE_INCLUDE
#define NSAPPLINKRPCV2_TRIGGERSOURCE_INCLUDE


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

///  Indicates the source from where the command was triggered.

  class TriggerSource
  {
  public:
    enum TriggerSourceInternal
    {
      INVALID_ENUM=-1,
      TS_MENU=0,
      TS_VR=1
    };
  
    TriggerSource() : mInternal(INVALID_ENUM)				{}
    TriggerSource(TriggerSourceInternal e) : mInternal(e)		{}
  
    TriggerSourceInternal get(void) const	{ return mInternal; }
    void set(TriggerSourceInternal e)		{ mInternal=e; }
  
  private:
    TriggerSourceInternal mInternal;
    friend class TriggerSourceMarshaller;
  };
  
}

#endif
