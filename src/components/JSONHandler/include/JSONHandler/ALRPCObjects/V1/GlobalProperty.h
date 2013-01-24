#ifndef NSAPPLINKRPC_GLOBALPROPERTY_INCLUDE
#define NSAPPLINKRPC_GLOBALPROPERTY_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
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
