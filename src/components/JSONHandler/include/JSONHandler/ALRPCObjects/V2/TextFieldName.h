#ifndef NSAPPLINKRPCV2_TEXTFIELDNAME_INCLUDE
#define NSAPPLINKRPCV2_TEXTFIELDNAME_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class TextFieldName
  {
  public:
    enum TextFieldNameInternal
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
