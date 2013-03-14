#ifndef NSAPPLINKRPCV2_SHOW_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_SHOW_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Image.h"
#include "SoftButton.h"
#include "TextAlignment.h"
#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Updates the persistent display. Supported fields depend on display capabilities.

  class Show_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    Show_request(const Show_request& c);
    Show_request(void);
    
    virtual ~Show_request(void);
  
    Show_request& operator =(const Show_request&);
  
    bool checkIntegrity(void);

    const std::string* get_mainField1(void) const;
    const std::string* get_mainField2(void) const;
    const std::string* get_mainField3(void) const;
    const std::string* get_mainField4(void) const;
    const TextAlignment* get_alignment(void) const;
    const std::string* get_statusBar(void) const;
    const std::string* get_mediaClock(void) const;
    const std::string* get_mediaTrack(void) const;
    const Image* get_graphic(void) const;
    const std::vector<SoftButton>* get_softButtons(void) const;
    const std::vector<std::string>* get_customPresets(void) const;

    void reset_mainField1(void);
    bool set_mainField1(const std::string& mainField1_);
    void reset_mainField2(void);
    bool set_mainField2(const std::string& mainField2_);
    void reset_mainField3(void);
    bool set_mainField3(const std::string& mainField3_);
    void reset_mainField4(void);
    bool set_mainField4(const std::string& mainField4_);
    void reset_alignment(void);
    bool set_alignment(const TextAlignment& alignment_);
    void reset_statusBar(void);
    bool set_statusBar(const std::string& statusBar_);
    void reset_mediaClock(void);
    bool set_mediaClock(const std::string& mediaClock_);
    void reset_mediaTrack(void);
    bool set_mediaTrack(const std::string& mediaTrack_);
    void reset_graphic(void);
    bool set_graphic(const Image& graphic_);
    void reset_softButtons(void);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);
    void reset_customPresets(void);
    bool set_customPresets(const std::vector<std::string>& customPresets_);

  private:
  
    friend class Show_requestMarshaller;


/**
     The text that should be displayed in a single or upper display line.
     If this text is not set, the text of mainField1 stays unchanged.
     If this text is empty "", the field will be cleared.
*/
      std::string* mainField1;	//!< (500)

/**
     The text that should be displayed on the second display line.
     If this text is not set, the text of mainField2 stays unchanged.
     If this text is empty "", the field will be cleared.
*/
      std::string* mainField2;	//!< (500)

/**
     The text that should be displayed on the second "page" first display line.
     If this text is not set, the text of mainField3 stays unchanged.
     If this text is empty "", the field will be cleared.
*/
      std::string* mainField3;	//!< (500)

/**
     The text that should be displayed on the second "page" second display line.
     If this text is not set, the text of mainField4 stays unchanged.
     If this text is empty "", the field will be cleared.
*/
      std::string* mainField4;	//!< (500)

/**
     Specifies how mainField1 and mainField2 texts should be aligned on display.
     If omitted, texts will be centered
*/
      TextAlignment* alignment;

///  Requires investigation regarding the nav display capabilities. Potentially lower lowerStatusBar, upperStatusBar, titleBar, etc.
      std::string* statusBar;	//!< (500)

/**
     Text value for MediaClock field. Has to be properly formatted by Mobile App according to Sync capabilities.
     If this text is set, any automatic media clock updates previously set with SetMediaClockTimer will be stopped.
*/
      std::string* mediaClock;	//!< (500)

/**
     The text that should be displayed in the track field.
     This field is only valid for media applications on NGN type ACMs.
     If this text is not set, the text of mediaTrack stays unchanged.
     If this text is empty "", the field will be cleared.
*/
      std::string* mediaTrack;	//!< (500)

/**
     Image struct determining whether static or dynamic image to display in app.
     If omitted on supported displays, the displayed graphic shall not change.
*/
      Image* graphic;

/**
     App defined SoftButtons.
     If omitted on supported displays, the currently displayed SoftButton values will not change.
*/
      std::vector<SoftButton>* softButtons;	//!<   [%s..%s] 

/**
     App labeled on-screen presets (i.e. GEN2).
     If omitted on supported displays, the presets will be shown as not defined.
*/
      std::vector<std::string>* customPresets;	//!<   [%s..%s] (500)
  };

}

#endif
