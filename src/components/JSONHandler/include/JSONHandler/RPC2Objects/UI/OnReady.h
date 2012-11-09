#ifndef RPC2COMMUNICATION_ONREADY
#define RPC2COMMUNICATION_ONREADY

#include "JSONHandler/RPC2Notification.h"

namespace RPC2Communication
{
  namespace UI
  {
    class OnReady : public RPC2Communication::RPC2Notification
    {
    public:
        OnReady();
        ~OnReady();    
    private:
        friend class OnReadyMarshaller;

    };
  }
}

#endif