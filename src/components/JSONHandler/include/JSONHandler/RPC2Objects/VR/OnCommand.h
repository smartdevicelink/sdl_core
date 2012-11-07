#ifndef RPC2COMMUNICATION_VR_ONCOMMAND_INCLUDE
#define RPC2COMMUNICATION_VR_ONCOMMAND_INCLUDE

#include "JSONHandler/RPC2Notification.h"


/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace VR
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
      unsigned int get_cmdID(void);


// setters
/// cmdID <= 2000000000
      bool set_cmdID(unsigned int cmdID);


    private:

      friend class OnCommandMarshaller;

      unsigned int cmdID;

    };
  }
}

#endif
