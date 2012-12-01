#ifndef NSRPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESETRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESETRESPONSE_INCLUDE

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

    class DeleteInteractionChoiceSetResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      DeleteInteractionChoiceSetResponse(const DeleteInteractionChoiceSetResponse& c);
      DeleteInteractionChoiceSetResponse(void);
    
      DeleteInteractionChoiceSetResponse& operator =(const DeleteInteractionChoiceSetResponse&);
    
      virtual ~DeleteInteractionChoiceSetResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class DeleteInteractionChoiceSetResponseMarshaller;


    };
  }
}

#endif
