#ifndef GLOBALPROPERTY_INCLUDE
#define GLOBALPROPERTY_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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

#endif
