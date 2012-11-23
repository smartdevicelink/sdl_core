#ifndef NSRPC2COMMUNICATION_UI_ONCOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONCOMMAND_INCLUDE

#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class OnCommand : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnCommand(const OnCommand& c);
      OnCommand(void);
    
      OnCommand& operator =(const OnCommand&);
    
      virtual ~OnCommand(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_commandId(void);


// setters
/// commandId <= 2000000000
      bool set_commandId(unsigned int commandId);


    private:

      friend class OnCommandMarshaller;

      unsigned int commandId;

    };
  }
}

#endif
