#ifndef NSAPPLINKRPCV2_SCROLLABLEMESSAGE_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_SCROLLABLEMESSAGE_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "SoftButton.h"
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

///  Creates a full screen overlay containing a large block of formatted text that can be scrolled with up to 8 SoftButtons defined

  class ScrollableMessage_request : public ALRPC2Message
  {
  public:
  
    ScrollableMessage_request(const ScrollableMessage_request& c);
    ScrollableMessage_request(void);
    
    virtual ~ScrollableMessage_request(void);
  
    ScrollableMessage_request& operator =(const ScrollableMessage_request&);
  
    bool checkIntegrity(void);

    const std::string& get_scrollableMessageBody(void) const;
    const unsigned int* get_timeout(void) const;
    const std::vector<SoftButton>* get_softButtons(void) const;

    bool set_scrollableMessageBody(const std::string& scrollableMessageBody_);
    void reset_timeout(void);
    bool set_timeout(unsigned int timeout_);
    void reset_softButtons(void);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);

  private:
  
    friend class ScrollableMessage_requestMarshaller;


///  Body of text that can include newlines and tabs.
      std::string scrollableMessageBody;	//!< (500)

///  App defined timeout.
      unsigned int* timeout;	//!<  (0,65535)

/**
     App defined SoftButtons.
     If omitted on supported displays, only the system defined "Close" SoftButton will be displayed.
*/
      std::vector<SoftButton>* softButtons;	//!<   [%s..%s] 
  };

}

#endif
