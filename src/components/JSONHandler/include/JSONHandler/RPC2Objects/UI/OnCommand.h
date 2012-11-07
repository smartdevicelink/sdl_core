#ifndef RPC2COMMUNICATION_UI_ONCOMMAND_INCLUDE
#define RPC2COMMUNICATION_UI_ONCOMMAND_INCLUDE

#include "JSONHandler/RPC2Notification.h"


/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class OnCommand : public ::RPC2Communication::RPC2Notification
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
