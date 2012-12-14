#ifndef NSRPC2COMMUNICATION_UI_ONREADY_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONREADY_INCLUDE

#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
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
