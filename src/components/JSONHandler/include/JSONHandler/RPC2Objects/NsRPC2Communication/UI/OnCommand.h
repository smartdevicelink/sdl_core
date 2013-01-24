#ifndef NSRPC2COMMUNICATION_UI_ONCOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONCOMMAND_INCLUDE

#include "JSONHandler/RPC2Notification.h"


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

      int get_appId(void);


// setters
/// commandId <= 2000000000
      bool set_commandId(unsigned int commandId);

      bool set_appId(int appId);


    private:

      friend class OnCommandMarshaller;

      unsigned int commandId;
      int appId;

    };
  }
}

#endif
