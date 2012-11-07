#ifndef APPLINKRPC_TEXTFIELDNAME_INCLUDE
#define APPLINKRPC_TEXTFIELDNAME_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
{

  class TextFieldName
  {
  public:
    enum TextFieldNameInternal
    {
      INVALID_ENUM=-1,

/**
     The first line of the main field of persistent display
     Applies to "Show"
*/
      mainField1=0,

/**
     The second line of the main field of persistent display
     Applies to "Show"
*/
      mainField2=1,

/**
     The status bar on Nav
     Applies to "Show"
*/
      statusBar=2,

/**
     Text value for MediaClock field. Has to be properly formatted by Mobile App according to Sync capabilities.
     Applies to "Show"
*/
      mediaClock=3,

/**
     The track field of NGN type ACMs.
     This field is only available for media applications.
     Applies to "Show"
*/
      mediaTrack=4,

/**
     The first line of the alert text field
     Applies to "Alert"
*/
      alertText1=5,

/**
     The second line of the alert text field
     Applies to "Alert"
*/
      alertText2=6
    };
  
    TextFieldName() : mInternal(INVALID_ENUM)				{}
    TextFieldName(TextFieldNameInternal e) : mInternal(e)		{}
  
    TextFieldNameInternal get(void) const	{ return mInternal; }
    void set(TextFieldNameInternal e)		{ mInternal=e; }
  
  private:
    TextFieldNameInternal mInternal;
    friend class TextFieldNameMarshaller;
  };
  
}

#endif
