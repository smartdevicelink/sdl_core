//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SCROLLABLEMESSAGERESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_SCROLLABLEMESSAGERESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class ScrollableMessageResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      ScrollableMessageResponse(const ScrollableMessageResponse& c);
      ScrollableMessageResponse(void);
    
      ScrollableMessageResponse& operator =(const ScrollableMessageResponse&);
    
      virtual ~ScrollableMessageResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class ScrollableMessageResponseMarshaller;


    };
  }
}

#endif
