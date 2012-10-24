#ifndef BUTTONEVENTMODE_INCLUDE
#define BUTTONEVENTMODE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class ButtonEventMode
{
public:
  enum ButtonEventModeInternal
  {
    INVALID_ENUM=-1,

///  A button has been released up
    BUTTONUP=0,

///  A button has been pressed down
    BUTTONDOWN=1
  };

  ButtonEventMode() : mInternal(INVALID_ENUM)				{}
  ButtonEventMode(ButtonEventModeInternal e) : mInternal(e)		{}

  ButtonEventModeInternal get(void) const	{ return mInternal; }
  void set(ButtonEventModeInternal e)		{ mInternal=e; }

private:
  ButtonEventModeInternal mInternal;
  friend class ButtonEventModeMarshaller;
};

#endif
