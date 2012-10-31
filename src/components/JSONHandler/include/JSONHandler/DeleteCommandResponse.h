#ifndef DELETECOMMANDRESPONSE_CLASS
#define DELETECOMMANDRESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class DeleteCommandResponse : public RPC2Response
    {
    public:
        DeleteCommandResponse();
        ~DeleteCommandResponse();
    
    private:
        friend class DeleteCommandResponseMarshaller;
    };
}

#endif