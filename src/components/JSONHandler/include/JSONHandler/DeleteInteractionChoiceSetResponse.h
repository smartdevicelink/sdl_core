#ifndef DELINTERCHOICESETRESPONSE
#define DELINTERCHOICESETRESPONSE

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class DeleteInteractionChoiceSetResponse : public RPC2Response
    {
    public:
        DeleteInteractionChoiceSetResponse();
        ~DeleteInteractionChoiceSetResponse();
    
    private:
        friend class DeleteInteractionChoiceSetResponseMarshaller;
    };
}

#endif