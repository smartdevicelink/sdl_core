#ifndef RESETGLOBALPROPERTIES_RESPONSE_CLASS
#define RESETGLOBALPROPERTIES_RESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class ResetGlobalPropertiesResponse : public RPC2Response
    {
    public:
        ResetGlobalPropertiesResponse();
        ~ResetGlobalPropertiesResponse();
    
    private:
        friend class ResetGlobalPropertiesResponseMarshaller;
    };
}

#endif