#ifndef PERFORMINTERACTIONRESPONSE
#define PERFORMINTERACTIONRESPONSE

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class PerformInteractionResponse : public RPC2Response
    {
    public:
        PerformInteractionResponse();
        ~PerformInteractionResponse();
    
    private:
        friend class PerformInteractionResponseMarshaller;
    };
}

#endif