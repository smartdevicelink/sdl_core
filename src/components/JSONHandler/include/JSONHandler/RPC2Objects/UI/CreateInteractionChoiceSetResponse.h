#ifndef RPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETRESPONSE_INCLUDE
#define RPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESETRESPONSE_INCLUDE

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

    class CreateInteractionChoiceSetResponse : public ::RPC2Communication::RPC2Response
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
