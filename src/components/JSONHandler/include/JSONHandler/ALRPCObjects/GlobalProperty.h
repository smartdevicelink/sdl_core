#ifndef NSAPPLINKRPC_GLOBALPROPERTY_INCLUDE
#define NSAPPLINKRPC_GLOBALPROPERTY_INCLUDE


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
