#ifndef NSAPPLINKRPCV2_GLOBALPROPERTY_INCLUDE
#define NSAPPLINKRPCV2_GLOBALPROPERTY_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
      TIMEOUTPROMPT=1,

///  The property vrHelpTitle of setGlobalProperties
      VRHELPTITLE=2,

///  The property array of vrHelp of setGlobalProperties
      VRHELPITEMS=3
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
