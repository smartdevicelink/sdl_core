#ifndef NSAPPLINKRPC_TRIGGERSOURCE_INCLUDE
#define NSAPPLINKRPC_TRIGGERSOURCE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
