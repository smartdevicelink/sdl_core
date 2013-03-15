//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ONREADY_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONREADY_INCLUDE

#include "JSONHandler/RPC2Notification.h"


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

    class OnReady : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnReady(const OnReady& c);
      OnReady(void);
    
      OnReady& operator =(const OnReady&);
    
      virtual ~OnReady(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class OnReadyMarshaller;


    };
  }
}

#endif
