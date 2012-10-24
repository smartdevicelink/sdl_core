#ifndef SETGLOBALPROPERTIES_RESPONSE_CLASS
#define SETGLOBALPROPERTIES_RESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class SetGlobalPropertiesResponse : public RPC2Response
    {
    public:
        SetGlobalPropertiesResponse();
        ~SetGlobalPropertiesResponse();
    
    private:
        friend class SetGlobalPropertiesResponseMarshaller;
    };
}

#endif