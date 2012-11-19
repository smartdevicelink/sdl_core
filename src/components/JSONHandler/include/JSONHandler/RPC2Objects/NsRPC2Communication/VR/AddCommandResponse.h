#ifndef NSRPC2COMMUNICATION_VR_ADDCOMMANDRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_VR_ADDCOMMANDRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Mon Nov 19 07:25:04 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VR
  {

    class AddCommandResponse : public ::RPC2Communication::RPC2Response
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
