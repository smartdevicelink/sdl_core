#ifndef DELETESUBMENURESPONSE_CLASS
#define DELETESUBMENURESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class DeleteSubMenuResponse : public RPC2Response
    {
    public:
        DeleteSubMenuResponse();
        ~DeleteSubMenuResponse();
    
    private:
        friend class DeleteSubMenuResponseMarshaller;
    };
}

#endif