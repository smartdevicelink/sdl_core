#ifndef UIADDSUBMENURESPONSECLASS
#define UIADDSUBMENURESPONSECLASS


#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class AddSubMenuResponse : public RPC2Response
    {
    public:
        AddSubMenuResponse();
        ~AddSubMenuResponse();
    
    private:
        friend class AddSubMenuResponseMarshaller;
    };
}

#endif