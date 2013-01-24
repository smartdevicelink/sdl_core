#ifndef NSRPC2COMMUNICATION_UI_SCROLLABLEMESSAGE_INCLUDE
#define NSRPC2COMMUNICATION_UI_SCROLLABLEMESSAGE_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V2/SoftButton.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class ScrollableMessage : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      ScrollableMessage(const ScrollableMessage& c);
      ScrollableMessage(void);
    
      ScrollableMessage& operator =(const ScrollableMessage&);
    
      virtual ~ScrollableMessage(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_scrollableMessageBody(void);

      const unsigned int* get_timeout(void);
      const std::vector< NsAppLinkRPCV2::SoftButton>* get_softButtons(void);
      int get_appId(void);


// setters
/// scrollableMessageBody <= 500
      bool set_scrollableMessageBody(const std::string& scrollableMessageBody);

/// timeout <= 65535
      bool set_timeout(const unsigned int& timeout);

      void reset_timeout(void);

/// 0 <= size <= 8
      bool set_softButtons(const std::vector< NsAppLinkRPCV2::SoftButton>& softButtons);

      void reset_softButtons(void);

      bool set_appId(int appId);


    private:

      friend class ScrollableMessageMarshaller;

      std::string scrollableMessageBody;
      unsigned int* timeout;
      std::vector< NsAppLinkRPCV2::SoftButton>* softButtons;
      int appId;

    };
  }
}

#endif
