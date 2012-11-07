#ifndef RPC2COMMUNICATION_UI_DELETECOMMANDRESPONSE_INCLUDE
#define RPC2COMMUNICATION_UI_DELETECOMMANDRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


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

    class DeleteCommandResponse : public ::RPC2Communication::RPC2Response
    {
    public:
    
      DeleteCommandResponse(const DeleteCommandResponse& c);
      DeleteCommandResponse(void);
    
      DeleteCommandResponse& operator =(const DeleteCommandResponse&);
    
      virtual ~DeleteCommandResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class DeleteCommandResponseMarshaller;


    };
  }
}

#endif
