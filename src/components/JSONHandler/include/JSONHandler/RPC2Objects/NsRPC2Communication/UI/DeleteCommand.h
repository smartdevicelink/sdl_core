#ifndef NSRPC2COMMUNICATION_UI_DELETECOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETECOMMAND_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
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


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);


    private:

      friend class DeleteCommandMarshaller;

      unsigned int cmdId;

    };
  }
}

#endif
