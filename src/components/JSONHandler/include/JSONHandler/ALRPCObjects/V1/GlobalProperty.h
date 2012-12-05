#ifndef NSAPPLINKRPC_GLOBALPROPERTY_INCLUDE
#define NSAPPLINKRPC_GLOBALPROPERTY_INCLUDE


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

///  The different global properties.

  class GlobalProperty
  {
  public:
    enum GlobalPropertyInternal
    {
      INVALID_ENUM=-1,

///  The property helpPrompt of setGlobalProperties
      HELPPROMPT=0,

///  The property timeoutPrompt of setGlobalProperties
      TIMEOUTPROMPT=1
    };
  
    GlobalProperty() : mInternal(INVALID_ENUM)				{}
    GlobalProperty(GlobalPropertyInternal e) : mInternal(e)		{}
  
    GlobalPropertyInternal get(void) const	{ return mInternal; }
    void set(GlobalPropertyInternal e)		{ mInternal=e; }
  
  private:
    GlobalPropertyInternal mInternal;
    friend class GlobalPropertyMarshaller;
  };
  
}

#endif
