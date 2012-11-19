#ifndef NSRPC2COMMUNICATION_UI_DELETECOMMANDRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETECOMMANDRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class DeleteCommandResponse : public ::NsRPC2Communication::RPC2Response
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
