#ifndef NSAPPLINKRPC_TEXTFIELDNAME_V2_INCLUDE
#define NSAPPLINKRPC_TEXTFIELDNAME_V2_INCLUDE


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

  class TextFieldName_v2
  {
  public:
    enum TextFieldName_v2Internal
    {
      INVALID_ENUM=-1,

/**
     The first line of first set of main fields of the persistent display
     Applies to "Show"
*/
      mainField1=0,

/**
     The second line of first set of main fields of the persistent display
     Applies to "Show"
*/
      mainField2=1,

/**
     The first line of second set of main fields of persistent display
     Applies to "Show"
*/
      mainField3=2,

/**
     The second line of second set of main fields of the persistent display
     Applies to "Show"
*/
      mainField4=3,

/**
     The status bar on Nav
     Applies to "Show"
*/
      statusBar=4,

/**
     Text value for MediaClock field.
     Applies to "Show"
*/
      mediaClock=5,

/**
     The track field of NGN type ACMs.
     This field is only available for media applications.
     Applies to "Show"
*/
      mediaTrack=6,

/**
     The first line of the alert text field
     Applies to "Alert"
*/
      alertText1=7,

/**
     The second line of the alert text field
     Applies to "Alert"
*/
      alertText2=8,

/**
     The third line of the alert text field
     Applies to "Alert"
*/
      alertText3=9,

/**
      Long form body of text that can include newlines and tabs.
     Applies to "ScrollableMessage"
*/
      scrollableMessageBody=10,

///   First line suggestion for a user response (in the case of VR enabled interaction
      initialInteractionText=11,

///   First line of navigation text
      navigationText1=12,

///   Second line of navigation text
      navigationText2=13,

///   Estimated Time of Arrival time for navigation
      ETA=14,

///   Total distance to destination for navigation
      totalDistance=15,

///   First line of text for audio pass thru
      audioPassThruDisplayText1=16,

///   Second line of text for audio pass thru
      audioPassThruDisplayText2=17,

///   Header text for slider
      sliderHeader=18,

///   Footer text for slider
      sliderFooter=19
    };
  
    TextFieldName_v2() : mInternal(INVALID_ENUM)				{}
    TextFieldName_v2(TextFieldName_v2Internal e) : mInternal(e)		{}
  
    TextFieldName_v2Internal get(void) const	{ return mInternal; }
    void set(TextFieldName_v2Internal e)		{ mInternal=e; }
  
  private:
    TextFieldName_v2Internal mInternal;
    friend class TextFieldName_v2Marshaller;
  };
  
}

#endif
