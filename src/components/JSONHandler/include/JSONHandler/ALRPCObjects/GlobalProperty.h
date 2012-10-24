#ifndef GLOBALPROPERTY_INCLUDE
#define GLOBALPROPERTY_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


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

#endif
