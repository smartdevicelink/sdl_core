#ifndef SCROLLABLEMESSAGE_REQUEST_INCLUDE
#define SCROLLABLEMESSAGE_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "SoftButton.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Creates a full screen overlay containing a large block of formatted text that can be scrolled with up to 8 SoftButtons defined

class ScrollableMessage_request : public ALRPCRequest
{
public:

  ScrollableMessage_request(const ScrollableMessage_request& c);
  ScrollableMessage_request(void);
  
  virtual ~ScrollableMessage_request(void);

  bool checkIntegrity(void);

  const std::string& get_scrollableMessageBody(void) const;
  unsigned int get_timeout(void) const;
  const std::vector<SoftButton>& get_softButtons(void) const;

  bool set_scrollableMessageBody(const std::string& scrollableMessageBody_);
  bool set_timeout(unsigned int timeout_);
  bool set_softButtons(const std::vector<SoftButton>& softButtons_);

private:

  friend class ScrollableMessage_requestMarshaller;

  std::string scrollableMessageBody;	//!< (500)
  unsigned int timeout;	//!<  (0,65535)
  std::vector<SoftButton> softButtons;	//!<   [%s..%s] 
};

#endif
