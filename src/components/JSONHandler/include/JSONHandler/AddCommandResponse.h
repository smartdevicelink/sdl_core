#ifndef ADDCOMMANDRESPONSE_CLASS
#define ADDCOMMANDRESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"
#include <string>

namespace RPC2Communication
{
    class AddCommandResponse : public RPC2Response
    {
    public:
        AddCommandResponse();
        ~AddCommandResponse();
    
    private:
        friend class AddCommandResponseMarshaller;
    };
}

#endif