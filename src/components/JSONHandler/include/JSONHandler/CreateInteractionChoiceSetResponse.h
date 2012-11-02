#ifndef CREATEINTERACTIONCHOICESETRESPONSE_CLASS
#define CREATEINTERACTIONCHOICESETRESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class CreateInteractionChoiceSetResponse : public RPC2Response
    {
    public:
        CreateInteractionChoiceSetResponse();
        ~CreateInteractionChoiceSetResponse();
    
    private:
        friend class CreateInteractionChoiceSetResponseMarshaller;
    };
}

#endif