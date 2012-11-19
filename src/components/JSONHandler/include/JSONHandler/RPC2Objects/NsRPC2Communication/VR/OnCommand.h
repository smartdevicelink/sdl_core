#ifndef NSRPC2COMMUNICATION_VR_ONCOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_VR_ONCOMMAND_INCLUDE

#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Mon Nov 19 12:18:27 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VR
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
