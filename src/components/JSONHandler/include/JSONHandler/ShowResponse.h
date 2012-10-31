#ifndef SHOW_RESPONSE_CLASS
#define SHOW_RESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class ShowResponse : public RPC2Response
    {
    public:
        ShowResponse();
        ~ShowResponse();
    
    private:
        friend class ShowResponseMarshaller;
    };
}

#endif