#ifndef ACTIVATEAPP_RESPONSE_CLASS
#define ACTIVATEAPP_RESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"
#include <string>
#include <vector>

namespace RPC2Communication 
{
    class ActivateAppResponse : public RPC2Response
    {
    public:
        ActivateAppResponse();
        ~ActivateAppResponse();
    
    private:
        friend class ActivateAppResponseMarshaller;
    
    };
}

#endif