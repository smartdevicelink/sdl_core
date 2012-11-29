#ifndef NSAPPLINKRPC_TRIGGERSOURCE_INCLUDE
#define NSAPPLINKRPC_TRIGGERSOURCE_INCLUDE


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
