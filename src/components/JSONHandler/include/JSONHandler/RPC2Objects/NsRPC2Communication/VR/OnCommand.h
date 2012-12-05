#ifndef NSRPC2COMMUNICATION_VR_ONCOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_VR_ONCOMMAND_INCLUDE

#include "JSONHandler/RPC2Notification.h"


/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
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

      int get_appId(void);


// setters
/// cmdID <= 2000000000
      bool set_cmdID(unsigned int cmdID);

      bool set_appId(int appId);


    private:

      friend class OnCommandMarshaller;

      unsigned int cmdID;
      int appId;

    };
  }
}

#endif
