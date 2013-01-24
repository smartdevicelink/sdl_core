#ifndef NSRPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESETRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESETRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
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
