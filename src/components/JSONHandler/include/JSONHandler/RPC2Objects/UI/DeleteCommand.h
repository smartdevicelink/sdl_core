#ifndef RPC2COMMUNICATION_UI_DELETECOMMAND_INCLUDE
#define RPC2COMMUNICATION_UI_DELETECOMMAND_INCLUDE

#include "JSONHandler/RPC2Request.h"


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

    class DeleteCommand : public ::RPC2Communication::RPC2Request
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
