#ifndef NSRPC2COMMUNICATION_UI_DELETECOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETECOMMAND_INCLUDE

#include "JSONHandler/RPC2Request.h"


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

    class DeleteCommand : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      DeleteCommand(const DeleteCommand& c);
      DeleteCommand(void);
    
      DeleteCommand& operator =(const DeleteCommand&);
    
      virtual ~DeleteCommand(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_cmdId(void);

      int get_appId(void);


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);

      bool set_appId(int appId);


    private:

      friend class DeleteCommandMarshaller;

      unsigned int cmdId;
      int appId;

    };
  }
}

#endif
