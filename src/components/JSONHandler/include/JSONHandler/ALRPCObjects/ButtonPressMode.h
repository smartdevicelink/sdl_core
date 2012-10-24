#ifndef BUTTONPRESSMODE_INCLUDE
#define BUTTONPRESSMODE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class ButtonPressMode
{
public:
  enum ButtonPressModeInternal
  {
    INVALID_ENUM=-1,

/**
     A button was released, after it was pressed for a long time
     Actual timing is defined by Sync and may vary
*/
    LONG=0,

/**
     A button was released, after it was pressed for a short time
     Actual timing is defined by Sync and may vary
*/
    SHORT=1
  };

  ButtonPressMode() : mInternal(INVALID_ENUM)				{}
  ButtonPressMode(ButtonPressModeInternal e) : mInternal(e)		{}

  ButtonPressModeInternal get(void) const	{ return mInternal; }
  void set(ButtonPressModeInternal e)		{ mInternal=e; }

private:
  ButtonPressModeInternal mInternal;
  friend class ButtonPressModeMarshaller;
};

#endif
