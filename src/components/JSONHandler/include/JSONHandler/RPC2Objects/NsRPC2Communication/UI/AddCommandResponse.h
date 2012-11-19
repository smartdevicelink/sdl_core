#ifndef NSRPC2COMMUNICATION_UI_ADDCOMMANDRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_ADDCOMMANDRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


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

    class AddCommandResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      AddCommandResponse(const AddCommandResponse& c);
      AddCommandResponse(void);
    
      AddCommandResponse& operator =(const AddCommandResponse&);
    
      virtual ~AddCommandResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class AddCommandResponseMarshaller;


    };
  }
}

#endif
