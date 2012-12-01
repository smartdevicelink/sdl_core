#ifndef NSAPPLINKRPC_GLOBALPROPERTY_V2_INCLUDE
#define NSAPPLINKRPC_GLOBALPROPERTY_V2_INCLUDE


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

  class GlobalProperty_v2
  {
  public:
    enum GlobalProperty_v2Internal
    {
      INVALID_ENUM=-1,

///  The property helpPrompt of setGlobalProperties
      HELPPROMPT=0,

///  The property timeoutPrompt of setGlobalProperties
      TIMEOUTPROMPT=1,

///  The property vrHelpTitle of setGlobalProperties
      VRHELPTITLE=2,

///  The property array of vrHelp of setGlobalProperties
      VRHELPITEMS=3
    };
  
    GlobalProperty_v2() : mInternal(INVALID_ENUM)				{}
    GlobalProperty_v2(GlobalProperty_v2Internal e) : mInternal(e)		{}
  
    GlobalProperty_v2Internal get(void) const	{ return mInternal; }
    void set(GlobalProperty_v2Internal e)		{ mInternal=e; }
  
  private:
    GlobalProperty_v2Internal mInternal;
    friend class GlobalProperty_v2Marshaller;
  };
  
}

#endif
