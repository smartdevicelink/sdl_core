#ifndef ALERT_RESPONSE_CLASS
#define ALERT_RESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{
    class AlertResponse : public RPC2Response
    {
    public:
        AlertResponse();
        ~AlertResponse();

    private:
        friend class AlertResponseMarshaller;
    };
}

#endif