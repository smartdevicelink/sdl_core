#ifndef NSAPPLINKRPCV2_SLIDER_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_SLIDER_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/ALRPC2Message.hh"


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

///  Creates a full screen or pop-up overlay (depending on platform) with a single user controlled slider.

  class Slider_request : public ALRPC2Message
  {
  public:
  
    Slider_request(const Slider_request& c);
    Slider_request(void);
    
    virtual ~Slider_request(void);
  
    Slider_request& operator =(const Slider_request&);
  
    bool checkIntegrity(void);

    unsigned int get_numTicks(void) const;
    unsigned int get_position(void) const;
    const std::string& get_sliderHeader(void) const;
    const std::vector<std::string>* get_sliderFooter(void) const;
    unsigned int get_timeout(void) const;

    bool set_numTicks(unsigned int numTicks_);
    bool set_position(unsigned int position_);
    bool set_sliderHeader(const std::string& sliderHeader_);
    void reset_sliderFooter(void);
    bool set_sliderFooter(const std::vector<std::string>& sliderFooter_);
    bool set_timeout(unsigned int timeout_);

  private:
  
    friend class Slider_requestMarshaller;


///  Number of selectable items on a horizontal axis
      unsigned int numTicks;	//!<  (2,26)

///  Initial position of slider control (cannot exceed numTicks)
      unsigned int position;	//!<  (1,26)

///  Text header to display
      std::string sliderHeader;	//!< (500)

/**
     Text footer to display (meant to display min/max threshold descriptors).
     For a static text footer, only one footer string shall be provided in the array.
     For a dynamic text footer, the number of footer text string in the array must match the numTicks value.
     For a dynamic text footer, text array string should correlate with potential slider position index.
     If omitted on supported displays, no footer text shall be displayed.
*/
      std::vector<std::string>* sliderFooter;	//!<   [%s..%s] (500)

///  App defined timeout.
      unsigned int timeout;	//!<  (0,65535)
  };

}

#endif
