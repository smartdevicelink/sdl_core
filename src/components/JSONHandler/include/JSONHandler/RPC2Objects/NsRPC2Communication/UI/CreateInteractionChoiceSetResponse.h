#ifndef NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class CreateInteractionChoiceSetResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      CreateInteractionChoiceSetResponse(const CreateInteractionChoiceSetResponse& c);
      CreateInteractionChoiceSetResponse(void);
    
      CreateInteractionChoiceSetResponse& operator =(const CreateInteractionChoiceSetResponse&);
    
      virtual ~CreateInteractionChoiceSetResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class CreateInteractionChoiceSetResponseMarshaller;


    };
  }
}

#endif
