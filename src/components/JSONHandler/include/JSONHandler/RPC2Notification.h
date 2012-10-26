#ifndef RPCS2_NOTIFICATION_CLASS
#define RPCS2_NOTIFICATION_CLASS value

#include "JSONHandler/RPC2Command.h"

namespace RPC2Communication
{    
    class RPC2Notification : public RPC2Command
    {
    public:
        RPC2Notification( );
        RPC2Notification( int method );
        ~RPC2Notification();

    };

}

#endif