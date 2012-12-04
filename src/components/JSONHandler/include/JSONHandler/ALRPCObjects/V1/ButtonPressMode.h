#ifndef NSAPPLINKRPCV1_BUTTONPRESSMODE_INCLUDE
#define NSAPPLINKRPCV1_BUTTONPRESSMODE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
{

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
  
}

#endif
